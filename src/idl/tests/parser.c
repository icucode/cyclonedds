/*
 * Copyright(c) 2020 ADLINK Technology Limited and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "idl/processor.h"

#include "CUnit/Theory.h"

static void
test_base_type(const char *str, uint32_t flags, int32_t retcode, idl_mask_t mask)
{
  idl_retcode_t ret;
  idl_pstate_t *pstate = NULL;
  idl_node_t *node;

  ret = idl_create_pstate(flags, NULL, &pstate);
  CU_ASSERT_EQUAL_FATAL(ret, IDL_RETCODE_OK);
  CU_ASSERT_PTR_NOT_NULL_FATAL(pstate);
  ret = idl_parse_string(pstate, str);
  CU_ASSERT(ret == retcode);
  if (ret != IDL_RETCODE_OK)
    goto bail;
  assert(pstate);
  node = pstate->root;
  CU_ASSERT_PTR_NOT_NULL(node);
  if (!node)
    goto bail;
  CU_ASSERT_EQUAL(idl_mask(node), IDL_STRUCT);
  if (idl_mask(node) == (IDL_DECLARATION | IDL_TYPE | IDL_STRUCT)) {
    idl_member_t *member = ((idl_struct_t *)node)->members;
    CU_ASSERT_PTR_NOT_NULL(member);
    if (!member)
      goto bail;
    CU_ASSERT_EQUAL(idl_mask(member), IDL_DECLARATION | IDL_MEMBER);
    CU_ASSERT_PTR_NOT_NULL(member->type_spec);
    if (!member->type_spec)
      goto bail;
    CU_ASSERT_EQUAL(idl_mask(member->type_spec), IDL_TYPE | mask);
    CU_ASSERT_PTR_NOT_NULL(member->declarators);
    if (!member->declarators)
      goto bail;
    CU_ASSERT(member->declarators->name && member->declarators->name->identifier);
    if (!member->declarators->name || !member->declarators->name->identifier)
      goto bail;
    CU_ASSERT_STRING_EQUAL(member->declarators->name->identifier, "c");
  }

bail:
  idl_delete_pstate(pstate);
}

#define T(type) "struct s{" type " c;};"

CU_TheoryDataPoints(idl_parser, base_types) = {
  CU_DataPoints(const char *,
    T("short"), T("unsigned short"),
    T("long"), T("unsigned long"),
    T("long long"), T("unsigned long long"),
    T("float"), T("double"), T("long double"),
    T("char"), T("wchar"),
    T("boolean"), T("octet")),
  CU_DataPoints(uint32_t,
    IDL_SHORT, IDL_USHORT,
    IDL_LONG, IDL_ULONG,
    IDL_LLONG, IDL_ULLONG,
    IDL_FLOAT, IDL_DOUBLE, IDL_LDOUBLE,
    IDL_CHAR, IDL_WCHAR,
    IDL_BOOL, IDL_OCTET)
};

CU_Theory((const char *s, uint32_t t), idl_parser, base_types)
{
  test_base_type(s, IDL_FLAG_EXTENDED_DATA_TYPES, 0, t);
}

CU_TheoryDataPoints(idl_parser, extended_base_types) = {
  CU_DataPoints(const char *, T("int8"), T("uint8"),
                              T("int16"), T("uint16"),
                              T("int32"), T("uint32"),
                              T("int64"), T("uint64")),
  CU_DataPoints(uint32_t, IDL_INT8, IDL_UINT8,
                          IDL_INT16, IDL_UINT16,
                          IDL_INT32, IDL_UINT32,
                          IDL_INT64, IDL_UINT64)
};
#undef T

CU_Theory((const char *s, uint32_t t), idl_parser, extended_base_types)
{
  test_base_type(s, IDL_FLAG_EXTENDED_DATA_TYPES, 0, t);
  test_base_type(s, 0u, IDL_RETCODE_SEMANTIC_ERROR, 0);
}

#define M(name, contents) "module " name " { " contents " };"
#define S(name, contents) "struct " name " { " contents " };"
#define LL(name) "long long " name ";"
#define LD(name) "long double " name ";"

CU_Test(idl_parser, embedded_module)
{
  idl_retcode_t ret;
  idl_pstate_t *pstate = NULL;
  idl_node_t *p;
  idl_module_t *m;
  idl_struct_t *s;
  idl_member_t *sm;
  const char str[] = M("foo", M("bar", S("baz", LL("foobar") LD("foobaz"))));

  ret = idl_create_pstate(0u, NULL, &pstate);
  CU_ASSERT_EQUAL_FATAL(ret, IDL_RETCODE_OK);
  CU_ASSERT_PTR_NOT_NULL_FATAL(pstate);
  ret = idl_parse_string(pstate, str);
  CU_ASSERT_EQUAL_FATAL(ret, IDL_RETCODE_OK);
  m = (idl_module_t*)pstate->root;
  CU_ASSERT_PTR_NOT_NULL_FATAL(m);
  assert(m);
  CU_ASSERT_PTR_NULL(idl_parent(m));
  //CU_ASSERT_PTR_NULL(idl_previous(m));
  CU_ASSERT_PTR_NULL(idl_next(m));
  CU_ASSERT_FATAL(idl_is_module(m));
  CU_ASSERT_STRING_EQUAL(idl_identifier(m), "foo");
  p = (idl_node_t*)m;
  m = (idl_module_t *)m->definitions;
  CU_ASSERT_PTR_NOT_NULL_FATAL(m);
  assert(p);
  CU_ASSERT_PTR_EQUAL(idl_parent(m), p);
  CU_ASSERT_PTR_NULL(idl_previous(m));
  CU_ASSERT_PTR_NULL(idl_next(m));
  CU_ASSERT_FATAL(idl_is_module(m));
  CU_ASSERT_STRING_EQUAL(idl_identifier(m), "bar");
  p = (idl_node_t*)m;
  s = (idl_struct_t *)m->definitions;
  CU_ASSERT_PTR_NOT_NULL_FATAL(s);
  assert(s);
  CU_ASSERT_PTR_EQUAL(idl_parent(s), p);
  CU_ASSERT_PTR_NULL(idl_previous(s));
  CU_ASSERT_PTR_NULL(idl_next(s));
  CU_ASSERT_FATAL(idl_is_struct(s));
  CU_ASSERT_STRING_EQUAL(idl_identifier(s), "baz");
  p = (idl_node_t*)s;
  sm = s->members;
  CU_ASSERT_PTR_NOT_NULL_FATAL(sm);
  assert(sm);
  CU_ASSERT_PTR_EQUAL(idl_parent(sm), p);
  CU_ASSERT_PTR_NULL(idl_previous(sm));
  CU_ASSERT_PTR_NOT_NULL_FATAL(idl_next(sm));
  CU_ASSERT_FATAL(idl_is_member(sm));
  CU_ASSERT(idl_type(sm->type_spec) == IDL_LLONG);
  CU_ASSERT(idl_is_declarator(sm->declarators));
  CU_ASSERT_STRING_EQUAL(idl_identifier(sm->declarators), "foobar");
  CU_ASSERT_PTR_EQUAL(sm, idl_previous(idl_next(sm)));
  sm = idl_next(sm);
  CU_ASSERT_PTR_EQUAL(idl_parent(sm), p);
  CU_ASSERT_PTR_NULL(idl_next(sm));
  CU_ASSERT_FATAL(idl_is_member(sm));
  CU_ASSERT(idl_type(sm->type_spec) == IDL_LDOUBLE);
  CU_ASSERT(idl_is_declarator(sm->declarators));
  CU_ASSERT_STRING_EQUAL(idl_identifier(sm->declarators), "foobaz");
  idl_delete_pstate(pstate);
}

#undef M
#undef S
#undef LL
#undef LD

// x. use already existing name

CU_Test(idl_parser, struct_in_struct_same_module)
{
  idl_retcode_t ret;
  idl_pstate_t *pstate = NULL;
  idl_module_t *m;
  idl_struct_t *s1, *s2;
  idl_member_t *s;
  const char str[] = "module m { struct s1 { char c; }; struct s2 { s1 s; }; };";

  ret = idl_create_pstate(0u, NULL, &pstate);
  CU_ASSERT_EQUAL_FATAL(ret, IDL_RETCODE_OK);
  CU_ASSERT_PTR_NOT_NULL_FATAL(pstate);
  assert(pstate);
  ret = idl_parse_string(pstate, str);
  CU_ASSERT_EQUAL_FATAL(ret, IDL_RETCODE_OK);
  m = (idl_module_t *)pstate->root;
  CU_ASSERT_FATAL(idl_is_module(m));
  assert(m);
  s1 = (idl_struct_t *)m->definitions;
  CU_ASSERT_FATAL(idl_is_struct(s1));
  assert(s1);
  s2 = idl_next(s1);
  CU_ASSERT_FATAL(idl_is_struct(s2));
  assert(s2);
  s = s2->members;
  CU_ASSERT_PTR_EQUAL(s->type_spec, s1);
  idl_delete_pstate(pstate);
}

CU_Test(idl_parser, struct_in_struct_other_module)
{
  idl_retcode_t ret;
  idl_pstate_t *pstate = NULL;
  idl_module_t *m1, *m2;
  idl_struct_t *s1, *s2;
  idl_member_t *s;
  const char str[] = "module m1 { struct s1 { char c; }; }; "
                     "module m2 { struct s2 { m1::s1 s; }; };";

  ret = idl_create_pstate(0u, NULL, &pstate);
  CU_ASSERT_EQUAL_FATAL(ret, IDL_RETCODE_OK);
  CU_ASSERT_PTR_NOT_NULL(pstate);
  assert(pstate);
  ret = idl_parse_string(pstate, str);
  CU_ASSERT_EQUAL_FATAL(ret, IDL_RETCODE_OK);
  m1 = (idl_module_t *)pstate->root;
  CU_ASSERT_FATAL(idl_is_module(m1));
  s1 = (idl_struct_t *)m1->definitions;
  CU_ASSERT_FATAL(idl_is_struct(s1));
  CU_ASSERT_PTR_EQUAL(s1->node.parent, m1);
  m2 = idl_next(m1);
  CU_ASSERT_FATAL(idl_is_module(m2));
  s2 = (idl_struct_t *)m2->definitions;
  CU_ASSERT_FATAL(idl_is_struct(s2));
  s = s2->members;
  CU_ASSERT_PTR_EQUAL(s->type_spec, s1);
  CU_ASSERT_PTR_EQUAL(s2->node.parent, m2);
  idl_delete_pstate(pstate);
}

// x. use nonexisting type!
// x. union with same declarators
// x. struct with same declarators
// x. struct with embedded struct
// x. struct with anonymous embedded struct
// x. constant expressions
// x. identifier that collides with a keyword

typedef struct rep_req_xcdr2 {
  const char *idl;
  bool req_xcdr2[4];
  size_t i;
} rep_req_xcdr2_t;

static idl_retcode_t test_req_xcdr2(const idl_pstate_t* pstate, const bool revisit, const idl_path_t* path, const void* node, void* user_data)
{
  (void) pstate;
  (void) revisit;
  (void) path;

  rep_req_xcdr2_t *test = (rep_req_xcdr2_t *)user_data;
  bool expected = test->req_xcdr2[test->i++];
  CU_ASSERT_EQUAL(idl_requires_xcdr2(node), expected);
  if (idl_requires_xcdr2(node) == expected)
    return IDL_RETCODE_OK;
  printf("required xcdr test failed\n");
  return IDL_RETCODE_SEMANTIC_ERROR;
}

static void test_require_xcdr2(rep_req_xcdr2_t *test)
{
  idl_pstate_t *pstate = NULL;
  idl_retcode_t ret = idl_create_pstate(IDL_FLAG_ANNOTATIONS, NULL, &pstate);
  CU_ASSERT_EQUAL_FATAL(ret, IDL_RETCODE_OK);
  CU_ASSERT_PTR_NOT_NULL_FATAL(pstate);
  if (!pstate)
    return;
  pstate->config.default_extensibility = IDL_FINAL;
  ret = idl_parse_string(pstate, test->idl);
  CU_ASSERT_EQUAL(ret, IDL_RETCODE_OK);

  idl_visitor_t visitor;
  memset(&visitor, 0, sizeof(visitor));
  visitor.visit = IDL_STRUCT | IDL_UNION;
  visitor.accept[IDL_ACCEPT_STRUCT] = &test_req_xcdr2;
  visitor.accept[IDL_ACCEPT_UNION] = &test_req_xcdr2;
  (void) idl_visit(pstate, pstate->root, &visitor, test);
  idl_delete_pstate(pstate);
}

#define S(ext,name,mem) ext " struct " name " { " mem " }; "
#define SB(ext,name,base,mem) ext " struct " name " : " base " { " mem " }; "
#define U(ext,name,mem) ext " union " name " switch (long) { " mem " }; "
#define E(ext,name,labels) ext " enum " name " { " labels " }; "
#define BM(ext,name,bits) ext " bitmask " name " { " bits " }; "
#define F "@final"
#define A "@appendable"
#define M "@mutable"
#define MEM_DEF "long f1;"
#define MEM_OPT "@optional long f1;"
#define MEM_EXT "@external long f1;"
#define LB_DEF "E1, E2, E3"
#define BITS_DEF "BM1, BM2, BM3"
#define UMEM_DEF "case 1: long f1;"

CU_Test(idl_parser, require_xcdr2)
{
  rep_req_xcdr2_t tests[] = {
    { S(F, "t", MEM_DEF), { false } },
    { S(F, "n", MEM_DEF) S(F, "t", "n f1;"), { false, false } },
    { S(F, "tb", MEM_DEF) SB(F, "t", "tb", ""), { false, false } },
    { S(F, "t", MEM_OPT), { true } },
    { S(F, "t", MEM_EXT), { false } },
    { S(A, "t", MEM_DEF), { true } },
    { S(M, "t", MEM_DEF), { true } },
    { S(M, "n", MEM_DEF) S(F, "t", "n f1;"), { true, true } },
    { S(F, "n", MEM_OPT) S(F, "t", "n f1;"), { true, true } },
    { S(F, "tb", MEM_OPT) SB(F, "t", "tb", ""), { true, true } },
    { E(A, "e", LB_DEF) S(F, "t", "e f1;"), { false } },
    { BM(A, "bm", BITS_DEF) S(F, "t", "bm f1;"), { false } },
    { S(M, "n", MEM_DEF) "typedef n td_n;" S(F, "t", "td_n f1;"), { true, true } },
    { S(M, "n", MEM_DEF) S(F, "t", "sequence<n> f1;"), { true, true } },
    { S(M, "n", MEM_DEF) S(F, "t", "n f1[3];"), { true, true } },
    { U(F, "u", UMEM_DEF), { false } },
    { U(A, "u", UMEM_DEF), { true } },
    { S(A, "n", MEM_DEF) U(F, "u", "case 1: n f1;"), { true, true } },
  };

  for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); i++) {
    printf("idl_parser_require_xcdr2 for idl: %s\n", tests[i].idl);
    test_require_xcdr2(&tests[i]);
  }
}

#undef S
#undef SB
#undef U
#undef E
#undef BM
#undef F
#undef A
#undef M
#undef MEM_DEF
#undef MEM_OPT
#undef MEM_EXT
#undef LB_DEF
#undef BITS_DEF
#undef UMEM_DEF
