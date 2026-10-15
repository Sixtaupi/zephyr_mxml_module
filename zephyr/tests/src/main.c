/* SPDX-License-Identifier: Apache-2.0 */

#include <mxml.h>
#include <zephyr/ztest.h>

extern int testmxml_main(int argc, char *argv[]);

ZTEST(mxml, test_parse_string)
{
	mxml_options_t *options = mxmlOptionsNew();
	mxml_node_t *tree;
	mxml_node_t *answer;

	zassert_not_null(options, "failed to allocate Mini-XML options");
	mxmlOptionsSetTypeValue(options, MXML_TYPE_INTEGER);

	tree = mxmlLoadString(NULL, options, "<root><answer>42</answer></root>");
	zassert_not_null(tree, "failed to parse XML");

	answer = mxmlFindPath(tree, "answer");
	zassert_not_null(answer, "failed to find parsed element");
	zassert_equal(mxmlGetInteger(answer), 42, "unexpected parsed value");

	mxmlDelete(tree);
	mxmlOptionsDelete(options);
}

ZTEST(mxml, test_upstream_suite)
{
	static char document[] =
		"<?xml version=\"1.0\"?>"
		"<root><value type=\"integer\">42</value></root>";
	char *argv[] = {
		"testmxml",
		document,
	};

	zassert_equal(testmxml_main(ARRAY_SIZE(argv), argv), 0,
		      "upstream testmxml suite failed");
}

ZTEST_SUITE(mxml, NULL, NULL, NULL, NULL, NULL);

