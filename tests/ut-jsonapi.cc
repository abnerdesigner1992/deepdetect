/**
 * DeepDetect
 * Copyright (c) 2014 Emmanuel Benazera
 * Author: Emmanuel Benazera <beniz@droidnik.fr>
 *
 * This file is part of deepdetect.
 *
 * deepdetect is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * deepdetect is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with deepdetect.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "deepdetect.h"
#include "jsonapi.h"
#include <gtest/gtest.h>
#include <iostream>

using namespace dd;

static std::string created_str = "{\"status\":{\"code\":201,\"msg\":\"Created\"}}";
static std::string bad_param_str = "{\"status\":{\"code\":400,\"msg\":\"BadRequest\"}}";
static std::string not_found_str = "{\"status\":{\"code\":404,\"msg\":\"NotFound\"}}";

TEST(jsonapi,service_create)
{
  JsonAPI japi;
  std::string sname = "my_service";
  std::string jstr = "{\"mllib\":\"caffe\",\"description\":\"my classifier\",\"type\":\"supervised\",\"model\":{\"repository\":\"here/\"},\"input\":\"image\"}";
  std::string joutstr = japi.service_create(sname,jstr);
  ASSERT_EQ(created_str,joutstr);

  rapidjson::Document jd;
  jd.Parse(jstr.c_str());
  ASSERT_TRUE(!jd.HasParseError());
  
  // service
  joutstr = japi.service_create("",jstr);
  ASSERT_EQ(not_found_str,joutstr);
  jd.Parse(jstr.c_str());
  std::string jstrt = japi.jrender(jd);

  // mllib
  jd.RemoveMember("mllib");
  jstrt = japi.jrender(jd);
  joutstr = japi.service_create(sname,jstrt);
  ASSERT_EQ(bad_param_str,joutstr);
  jd.Parse(jstr.c_str());

  // description
  jd.RemoveMember("description");
  jstrt = japi.jrender(jd);
  joutstr = japi.service_create(sname,jstrt);
  ASSERT_EQ(created_str,joutstr);
  jd.Parse(jstr.c_str());

  // type
  jd.RemoveMember("type");
  jstrt = japi.jrender(jd);
  joutstr = japi.service_create(sname,jstrt);
  ASSERT_EQ(created_str,joutstr);
  jd.Parse(jstr.c_str());

  // for Caffe
  // model
  
  // input
  jd.RemoveMember("input");
  jstrt = japi.jrender(jd);
  joutstr = japi.service_create(sname,jstrt);
  ASSERT_EQ(bad_param_str,joutstr);
  jd.Parse(jstr.c_str());
  
}