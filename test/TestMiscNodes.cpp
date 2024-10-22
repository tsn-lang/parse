#include "Common.h"
#include <parse/misc/IdentifierNode.h>

using namespace tokenize;
using namespace parse; 

TEST_CASE("IdentifierNode", "[parse]") {
    TestContext test("test");
    IdentifierNode* n = IdentifierNode::TryParse(test.ctx);
    REQUIRE(n != nullptr);
    REQUIRE(n->text == "test");
    REQUIRE(n->isError() == false);
    REQUIRE(test.ctx->getState()->messages.size() == 0);
}