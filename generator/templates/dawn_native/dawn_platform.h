//* Copyright 2021 The Dawn Authors
//*
//* Licensed under the Apache License, Version 2.0 (the "License");
//* you may not use this file except in compliance with the License.
//* You may obtain a copy of the License at
//*
//*     http://www.apache.org/licenses/LICENSE-2.0
//*
//* Unless required by applicable law or agreed to in writing, software
//* distributed under the License is distributed on an "AS IS" BASIS,
//* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//* See the License for the specific language governing permissions and
//* limitations under the License.

{% set namespace_name = Name(metadata.native_namespace) %}
{% set NATIVE_DIR = namespace_name.concatcase().upper() %}
{% set PREFIX = metadata.proc_table_prefix.upper() %}
#ifndef {{NATIVE_DIR}}_{{PREFIX}}_PLATFORM_AUTOGEN_H_
#define {{NATIVE_DIR}}_{{PREFIX}}_PLATFORM_AUTOGEN_H_

{% set api = metadata.api.lower() %}
#include "dawn/{{api}}_cpp.h"
{% set impl_dir = metadata.impl_dir + "/" if metadata.impl_dir else "" %}
{% set native_namespace = namespace_name.snake_case() %}
{% set native_dir = impl_dir + native_namespace %}
#include "{{native_dir}}/Forward.h"

{% set namespace = metadata.namespace %}
// Use our autogenerated version of the {{namespace}} structures that point to {{native_namespace}} object types
// (wgpu::Buffer is dawn_native::BufferBase*)
#include <{{native_dir}}/{{namespace}}_structs_autogen.h>

namespace {{native_namespace}} {

    {% for type in by_category["structure"] %}
        inline const {{as_cType(type.name)}}* ToAPI(const {{as_cppType(type.name)}}* rhs) {
            return reinterpret_cast<const {{as_cType(type.name)}}*>(rhs);
        }

        inline {{as_cType(type.name)}}* ToAPI({{as_cppType(type.name)}}* rhs) {
            return reinterpret_cast<{{as_cType(type.name)}}*>(rhs);
        }

        inline const {{as_cppType(type.name)}}* FromAPI(const {{as_cType(type.name)}}* rhs) {
            return reinterpret_cast<const {{as_cppType(type.name)}}*>(rhs);
        }

        inline {{as_cppType(type.name)}}* FromAPI({{as_cType(type.name)}}* rhs) {
            return reinterpret_cast<{{as_cppType(type.name)}}*>(rhs);
        }
    {% endfor %}

    {% for type in by_category["object"] %}
        inline const {{as_cType(type.name)}}Impl* ToAPI(const {{as_cppType(type.name)}}Base* rhs) {
            return reinterpret_cast<const {{as_cType(type.name)}}Impl*>(rhs);
        }

        inline {{as_cType(type.name)}}Impl* ToAPI({{as_cppType(type.name)}}Base* rhs) {
            return reinterpret_cast<{{as_cType(type.name)}}Impl*>(rhs);
        }

        inline const {{as_cppType(type.name)}}Base* FromAPI(const {{as_cType(type.name)}}Impl* rhs) {
            return reinterpret_cast<const {{as_cppType(type.name)}}Base*>(rhs);
        }

        inline {{as_cppType(type.name)}}Base* FromAPI({{as_cType(type.name)}}Impl* rhs) {
            return reinterpret_cast<{{as_cppType(type.name)}}Base*>(rhs);
        }
    {% endfor %}

    template <typename T>
    struct EnumCount;

    {% for e in by_category["enum"] if e.contiguousFromZero %}
        template<>
        struct EnumCount<{{namespace}}::{{as_cppType(e.name)}}> {
            static constexpr uint32_t value = {{len(e.values)}};
        };
    {% endfor %}
}

#endif  // {{NATIVE_DIR}}_{{PREFIX}}_PLATFORM_AUTOGEN_H_