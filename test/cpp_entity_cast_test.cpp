// Copyright (C) 2017 Manu Sanchez <Manu343726@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#include <cppast/cpp_class.hpp>
#include <cppast/cpp_type_alias.hpp>
#include <cppast/cpp_entity_cast.hpp>

#include "test_parser.hpp"

using namespace cppast;

TEST_CASE("cpp_entity_cast")
{
    auto code = R"(

/// using mytypedef=void;
using mytypedef = void;

/// class foo{
/// };
class foo{};
)";

    cpp_entity_index idx;
    auto file = parse(idx, "cpp_enum.cpp", code);
    const cpp_entity* alias = nullptr;
    const cpp_entity* class_decl = nullptr;
    std::size_t count = test_visit<cpp_type_alias>(*file, [&](const cpp_type_alias& entity)
    {
        alias = &entity;
    });
    count += test_visit<cpp_class>(*file, [&](const cpp_class& entity)
    {
        class_decl = &entity;
    });

    REQUIRE(alias != nullptr);
    REQUIRE(class_decl != nullptr);

    REQUIRE(cpp_entity_cast<cpp_type_alias>(alias) == static_cast<const cpp_type_alias*>(alias));
    REQUIRE(cpp_entity_cast<cpp_class>(class_decl) == static_cast<const cpp_class*>(class_decl));
    REQUIRE(cpp_entity_cast<cpp_class>(alias) == nullptr);
    REQUIRE(cpp_entity_cast<cpp_type_alias>(class_decl) == nullptr);

    REQUIRE(&cpp_entity_cast<cpp_type_alias>(*alias) == static_cast<const cpp_type_alias*>(alias));
    REQUIRE(&cpp_entity_cast<cpp_class>(*class_decl) == static_cast<const cpp_class*>(class_decl));
    REQUIRE_THROWS_AS(cpp_entity_cast<cpp_class>(*alias), bad_cpp_entity_cast);
    REQUIRE_THROWS_AS(cpp_entity_cast<cpp_type_alias>(*class_decl), bad_cpp_entity_cast);

    REQUIRE(count == 2u);
}
