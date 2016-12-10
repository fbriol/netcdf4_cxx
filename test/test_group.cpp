/* This file is part of NetCDF4_CXX library.

   NetCDF4_CXX is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   NetCDF4_CXX is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with NetCDF4_CXX.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <netcdf.h>
#include <stddef.h>
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <list>
#include <netcdf4_cxx/abstract_dataset.hpp>
#include <netcdf4_cxx/attribute.hpp>
#include <netcdf4_cxx/dataset.hpp>
#include <netcdf4_cxx/dimension.hpp>
#include <netcdf4_cxx/group.hpp>
#include <netcdf4_cxx/netcdf.hpp>
#include <netcdf4_cxx/type.hpp>
#include <netcdf4_cxx/variable.hpp>
#include <string>
#include <vector>

#include "tempfile.hpp"

BOOST_AUTO_TEST_SUITE(test_group)

BOOST_AUTO_TEST_CASE(test_accessors) {
  Object object;
  netcdf::type::UnsignedInt64 int64(object);
  netcdf::Group group(object);
  group.AddAttribute("att1").WriteText("mytext");
  group.AddDimension("x", 10);
  group.AddUnlimitedDimension("y");
  std::vector<netcdf::Dimension> dims = group.GetDimensions();
  auto var1 = group.AddVariable("var1", netcdf::type::Int(object), dims);
  auto var2 = group.AddVariable("var2", netcdf::type::Int(object), dims);
  auto last = group.AddGroup("a").AddGroup("b").AddGroup("c");

  // Compound type Coordinate2D
  struct Coordinate2D {
    size_t i;
    size_t j;
  };
  netcdf::type::Compound type(object, "Coordinate2D", sizeof(Coordinate2D));
  type.InsertMember("i", NC_COMPOUND_OFFSET(Coordinate2D, i), int64);
  type.InsertMember("j", NC_COMPOUND_OFFSET(Coordinate2D, j), int64);

  // Compound type Coordinate3D
  struct Coordinate3D {
    size_t i;
    size_t j;
    size_t k;
  };
  type = netcdf::type::Compound(object, "Coordinate3D", sizeof(Coordinate3D));
  type.InsertMember("i", NC_COMPOUND_OFFSET(Coordinate3D, i), int64);
  type.InsertMember("j", NC_COMPOUND_OFFSET(Coordinate3D, j), int64);
  type.InsertMember("k", NC_COMPOUND_OFFSET(Coordinate3D, k), int64);

  BOOST_CHECK_EQUAL(last.GetShortName(), "c");
  BOOST_CHECK_EQUAL(last.GetLongName(), "/a/b/c");
  BOOST_CHECK_EQUAL(last.GetGroups().size(), 0);

  BOOST_CHECK_EQUAL(last.IsRoot(), false);
  BOOST_CHECK_EQUAL(group.IsRoot(), true);

  std::list<netcdf::Group> groups = group.GetGroups();
  BOOST_REQUIRE(groups.size() == 1);
  BOOST_CHECK_EQUAL(groups.back().GetShortName(), "a");

  std::list<std::string> ref({"/", "/a", "/a/b", "/a/b/c"});
  std::list<std::string> res;
  for (auto& item : last) {
    res.push_front(item.GetLongName());
  }
  BOOST_CHECK_EQUAL_COLLECTIONS(ref.begin(), ref.end(), res.begin(), res.end());

  res.clear();
  const netcdf::Group& cst_last = last;
  for (auto& item : cst_last) {
    res.push_front(item.GetLongName());
  }
  BOOST_CHECK_EQUAL_COLLECTIONS(ref.begin(), ref.end(), res.begin(), res.end());

  BOOST_CHECK(group.FindDimension("") == nullptr);
  BOOST_CHECK(group.FindDimension("x") != nullptr);

  BOOST_CHECK_EQUAL(group.GetNamedGroup("a").id(),
                    last.GetParentGroup().GetParentGroup().id());

  BOOST_CHECK_EQUAL(group.GetVariables().size(), 2);
  BOOST_CHECK(group.FindVariable("") == nullptr);
  BOOST_CHECK(group.FindVariable("var1") != nullptr);

  std::list<netcdf::type::Generic> types = group.GetDataTypes();
  BOOST_CHECK_EQUAL(types.size(), 2);
  BOOST_CHECK(last.FindDataType("Coordinate3D") != nullptr);
  BOOST_CHECK(last.FindDataType("Coordinate1D") == nullptr);
}

BOOST_AUTO_TEST_CASE(test_walk) {
  Object object;
  netcdf::type::UnsignedInt64 int64(object);
  netcdf::Group root(object);
  netcdf::Group a = root.AddGroup("a");
  netcdf::Group b = root.AddGroup("b");
  netcdf::Group c = b.AddGroup("c");
  netcdf::Group d = b.AddGroup("d");
  netcdf::Group e = d.AddGroup("e");
  netcdf::Group f = d.AddGroup("f");
  netcdf::Group g = f.AddGroup("e");
  netcdf::Group h = f.AddGroup("g");

  a.AddDimension("ad", 10);
  b.AddDimension("bd", 10);

  auto ad = a.GetDimensions();
  auto bd = b.GetDimensions();

  netcdf::Variable av = a.AddVariable("av", int64, ad);
  netcdf::Variable bv = b.AddVariable("bv", int64, bd);
  netcdf::Variable cv = c.AddVariable("cv", int64);
  netcdf::Variable dv = d.AddVariable("dv", int64);
  netcdf::Variable ev = e.AddVariable("ev", int64);
  netcdf::Variable fv = f.AddVariable("fv", int64);
  netcdf::Variable gv = g.AddVariable("gv", int64);
  netcdf::Variable hv = h.AddVariable("hv", int64);

  BOOST_CHECK_EQUAL(a.GetLongName(), "/a");
  BOOST_CHECK_EQUAL(b.GetLongName(), "/b");
  BOOST_CHECK_EQUAL(c.GetLongName(), "/b/c");
  BOOST_CHECK_EQUAL(d.GetLongName(), "/b/d");
  BOOST_CHECK_EQUAL(e.GetLongName(), "/b/d/e");
  BOOST_CHECK_EQUAL(f.GetLongName(), "/b/d/f");
  BOOST_CHECK_EQUAL(g.GetLongName(), "/b/d/f/e");
  BOOST_CHECK_EQUAL(h.GetLongName(), "/b/d/f/g");

  // Compound type Coordinate2D
  struct Coordinate2D {
    size_t i;
    size_t j;
  };

  netcdf::type::Compound at(a, "at", sizeof(Coordinate2D));
  at.InsertMember("i", NC_COMPOUND_OFFSET(Coordinate2D, i), int64);
  at.InsertMember("j", NC_COMPOUND_OFFSET(Coordinate2D, j), int64);

  netcdf::type::Compound ct(c, "ct", sizeof(Coordinate2D));
  ct.InsertMember("i", NC_COMPOUND_OFFSET(Coordinate2D, i), int64);
  ct.InsertMember("j", NC_COMPOUND_OFFSET(Coordinate2D, j), int64);

  netcdf::type::Compound et(e, "et", sizeof(Coordinate2D));
  et.InsertMember("i", NC_COMPOUND_OFFSET(Coordinate2D, i), int64);
  et.InsertMember("j", NC_COMPOUND_OFFSET(Coordinate2D, j), int64);

  netcdf::type::Compound gt(g, "gt", sizeof(Coordinate2D));
  gt.InsertMember("i", NC_COMPOUND_OFFSET(Coordinate2D, i), int64);
  gt.InsertMember("j", NC_COMPOUND_OFFSET(Coordinate2D, j), int64);

  auto childs = root.Walk();
  BOOST_CHECK_EQUAL(childs.size(), 8);

  auto types = root.GetDataTypes();
  BOOST_CHECK_EQUAL(types.size(), 4);

  BOOST_CHECK_EQUAL(root.GetGroup().id(), root.id());
  BOOST_CHECK_EQUAL(a.GetParentGroup().id(), root.id());
  BOOST_CHECK_THROW(root.GetParentGroup(), std::logic_error);
  BOOST_CHECK_EQUAL(h.GetRootGroup().id(), root.id());

  BOOST_CHECK_EQUAL(g.GetCommonParent(d)->GetShortName(), "b");
  BOOST_CHECK(a.GetCommonParent(b) == nullptr);

  BOOST_CHECK(g.FindVariable("bv") == nullptr);
  BOOST_CHECK(g.FindVariableOrInParent("bv") != nullptr);

  BOOST_CHECK(a.FindDimensionLocal("ad") != nullptr);
  BOOST_CHECK(g.FindDimension("ad") == nullptr);
  BOOST_CHECK(g.FindDimension("bd") != nullptr);

  BOOST_CHECK(g.FindGroup("e") == nullptr);
  BOOST_CHECK(b.FindGroup("c") != nullptr);
  BOOST_CHECK(d.FindGroup("c") == nullptr);
}

BOOST_AUTO_TEST_CASE(test_split) {
  auto result = netcdf::Group::SplitGroupsAndVariable("a/b/c");
  BOOST_REQUIRE(result.first.size() == 2);
  BOOST_CHECK_EQUAL(result.first.front(), "a");
  result.first.pop_front();
  BOOST_CHECK_EQUAL(result.first.front(), "b");
  BOOST_CHECK_EQUAL(result.second, "c");
  
  result = netcdf::Group::SplitGroupsAndVariable("a/b/");
  BOOST_REQUIRE(result.first.size() == 2);
  BOOST_CHECK_EQUAL(result.first.front(), "a");
  result.first.pop_front();
  BOOST_CHECK_EQUAL(result.first.front(), "b");
  BOOST_CHECK_EQUAL(result.second, "");

  result = netcdf::Group::SplitGroupsAndVariable("/");
  BOOST_REQUIRE(result.first.size() == 1);
  BOOST_CHECK_EQUAL(result.first.front(), "");
  BOOST_CHECK_EQUAL(result.second, "");
}

BOOST_AUTO_TEST_SUITE_END()
