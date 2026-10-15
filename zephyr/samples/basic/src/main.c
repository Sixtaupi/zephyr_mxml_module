/* SPDX-License-Identifier: Apache-2.0 */

#include <stdio.h>
#include <string.h>

#include <mxml.h>

static mxml_type_t value_type(void *cbdata, mxml_node_t *node)
{
	const char *element = mxmlGetElement(node);

	(void)cbdata;

	if ((element != NULL) && (strcmp(element, "value") == 0)) {
		return MXML_TYPE_INTEGER;
	}

	return MXML_TYPE_OPAQUE;
}

int main(void)
{
	static const char document[] =
		"<settings>"
		"<name>zephyr</name>"
		"<value>42</value>"
		"</settings>";
	mxml_options_t *options;
	mxml_node_t *tree;
	mxml_node_t *value;
	mxml_node_t *status;
	char output[256];
	size_t output_len;
	int ret = 0;

	options = mxmlOptionsNew();
	if (options == NULL) {
		fprintf(stderr, "Unable to allocate Mini-XML options\n");
		return 1;
	}

	mxmlOptionsSetTypeCallback(options, value_type, NULL);
	tree = mxmlLoadString(NULL, options, document);
	if (tree == NULL) {
		fprintf(stderr, "Unable to parse the XML document\n");
		ret = 1;
		goto out_options;
	}

	value = mxmlFindPath(tree, "value");
	if (value == NULL) {
		fprintf(stderr, "Unable to find the value element\n");
		ret = 1;
		goto out_tree;
	}

	printf("Mini-XML parsed value: %ld\n", mxmlGetInteger(value));

	if (!mxmlSetInteger(value, 84)) {
		fprintf(stderr, "Unable to update the value element\n");
		ret = 1;
		goto out_tree;
	}

	status = mxmlNewElement(tree, "status");
	if ((status == NULL) || (mxmlNewOpaque(status, "updated") == NULL)) {
		fprintf(stderr, "Unable to add the status element\n");
		ret = 1;
		goto out_tree;
	}

	mxmlOptionsSetWrapMargin(options, 0);
	output_len = mxmlSaveString(tree, options, output, sizeof(output));
	if ((output_len == 0) || (output_len >= sizeof(output))) {
		fprintf(stderr, "Unable to serialize the XML document\n");
		ret = 1;
		goto out_tree;
	}

	printf("Modified XML: %s\n", output);

out_tree:
	mxmlDelete(tree);
out_options:
	mxmlOptionsDelete(options);

	return ret;
}

