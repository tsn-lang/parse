@echo off

set /p node_name="Enter node name: "

set header_name="%node_name%.h"
set src_name="%node_name%.cpp"

set printf="C:\Program Files\Git\usr\bin\printf.exe"

cd "./include/parse/expressions"
%printf% "#pragma once\n" >> %header_name%
%printf% "#include <parse/misc/Node.h>\n\n" >> %header_name%
%printf% "namespace parse {\n" >> %header_name%
%printf% "    class Context;\n\n" >> %header_name%
%printf% "    class %node_name% : public Node {\n" >> %header_name%
%printf% "        public:\n" >> %header_name%
%printf% "            virtual void acceptVisitor(INodeVisitor* visitor);\n" >> %header_name%
%printf% "            static %node_name%* Create(Context* ctx);\n" >> %header_name%
%printf% "            static %node_name%* TryParse(Context* ctx);\n\n" >> %header_name%
%printf% "        private:\n" >> %header_name%
%printf% "            %node_name%(Context* ctx);\n" >> %header_name%
%printf% "    };\n" >> %header_name%
%printf% "};" >> %header_name%

cd "../../../src/expressions"
%printf% "#include <parse/expressions/%header_name%>\n" >> %src_name%
%printf% "#include <parse/Context.h>\n\n" >> %src_name%
%printf% "namespace parse {\n" >> %src_name%
%printf% "    %node_name%::%node_name%(Context* ctx) : Node(ctx) {}\n" >> %src_name%
%printf% "    void %node_name%::acceptVisitor(INodeVisitor* visitor) { visitor->visit(this); }\n" >> %src_name%
%printf% "    %node_name%* %node_name%::Create(Context* ctx) { return new (ctx->allocNode()) %node_name%(ctx); }\n\n" >> %src_name%
%printf% "    %node_name%* %node_name%::TryParse(Context* ctx) {\n" >> %src_name%
%printf% "        if (ctx->atEnd()) return nullptr;\n\n" >> %src_name%
%printf% "        return nullptr;\n" >> %src_name%
%printf% "    }\n" >> %src_name%
%printf% "};" >> %src_name%

cd "../../"
copy /b CMakeLists.txt +,,