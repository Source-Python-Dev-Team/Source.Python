# Copyright 2012 John Graettinger. All rights reserved

import sys
import os
import msvcrt

import plugin_pb2
from google.protobuf.descriptor_pb2 import \
    FieldDescriptorProto as FieldDesc

def generate_enum(enum_full_name, enum_type, cpp_header, package):

    source = [MSG_HEAD % cpp_header]
    args = {'enum_name': enum_full_name}

    # namespace components
    for component in package:
        source.append('namespace %s {' % component)

    # begin enum definition
    source.append(ENUM_DEFINITION % args)

    # value enumeration
    for value in enum_type.value:

        args['value_name'] = value.name
        source.append(ENUM_VALUE % args)

    source.append(';\n}\n')

    # close namespace
    for component in package:
        source.append('}')

    return source

def generate_message(msg_full_name, msg_type, cpp_header, package):

    source = [MSG_HEAD % cpp_header]
    args = {'msg_name': msg_full_name}

    # namespace components
    for component in package:
        source.append('namespace %s {' % component)

    # field methods enumeration
    for field in msg_type.field:

        args['field_name'] = field.name.lower()

        if field.type in cpp_type_map:
            args['cpp_type'] = cpp_type_map[field.type]
        else:
            package_prefix = '.%s' % ''.join('%s.' % p for p in package)

            # field_type is the protobuf typename, minus the leading package
            #   prefix, and with '.' replaced by '_'
            field_type = field.type_name.replace(package_prefix, '', 1
                ).replace('.', '_')

            # Message instances are passed around by pointer
            args['cpp_type'] = '%s *' % field_type

        key = (field.label, type_map[field.type])

        template = method_templates.get(key)

        if template:
            source.append(template % args)

    # binding class definition
    source.append(MSG_DEFINITION % args)

    # field bindings enumeration
    for field in msg_type.field:

        args['field_name'] = field.name.lower()
        key = (field.label, type_map[field.type])

        template = binding_templates.get(key)

        if template:
            source.append(template % args)
        else:
            print >> sys.stderr, "Don't know how to generate "\
                "bindings for %s" % str(field)

    source.append(';\n}\n')

    # close namespace
    for component in package:
        source.append('}')

    return source

def generate_module(module_name, generated_names, package):

    source = ["#include <boost/python.hpp>", "#include <boost/python/scope.hpp>", 
    "#include <google/protobuf/message.h>", "#include \"../../export_main.h\"\n"]

    # namespace components
    for component in package:
        source.append('namespace %s {' % component)

    for name in generated_names:
        source.append('    void make_%s_bindings();' % name)

    for component in package:
        source.append('}')

    source.append('\nDECLARE_SP_MODULE(%s)' % module_name.split("/")[-1].title())
    source.append('{')
    
    source.append('    {')
    source.append('        class dummy_scope_google{};')
    source.append('        scope google = class_<dummy_scope_google>("google");')
    source.append('        {')
    source.append('            class dummy_scope_protobuf{};')
    source.append('            scope protobuf = class_<class dummy_scope_protobuf>("protobuf");')
    source.append('            {')
    source.append('                boost::python::class_<google::protobuf::Message, boost::noncopyable> binder("Message", no_init);')
    source.append('                binder.def("CopyFrom", &google::protobuf::Message::CopyFrom);')
    source.append('                binder.def("MergeFrom", &google::protobuf::Message::MergeFrom);')
    source.append('                binder.def("FindInitializationErrors", &google::protobuf::Message::FindInitializationErrors);')
    source.append('                binder.def("InitializationErrorString", &google::protobuf::Message::InitializationErrorString);')
    source.append('                binder.def("DiscardUnknownFields", &google::protobuf::Message::DiscardUnknownFields);')
    source.append('                binder.def("SpaceUsed", &google::protobuf::Message::SpaceUsed);')
    source.append('                binder.def("DebugString", &google::protobuf::Message::DebugString);')
    source.append('                binder.def("ShortDebugString", &google::protobuf::Message::ShortDebugString);')
    source.append('                binder.def("Utf8DebugString", &google::protobuf::Message::Utf8DebugString);')
    source.append('                binder.def("PrintDebugString", &google::protobuf::Message::PrintDebugString);')
    source.append('                binder.def("ParseFromFileDescriptor", &google::protobuf::Message::ParseFromFileDescriptor);')
    source.append('                binder.def("ParsePartialFromFileDescriptor", &google::protobuf::Message::ParsePartialFromFileDescriptor);')
    source.append('                binder.def("ParseFromIstream", &google::protobuf::Message::ParseFromIstream);')
    source.append('                binder.def("ParsePartialFromIstream", &google::protobuf::Message::ParsePartialFromIstream);')
    source.append('                binder.def("SerializeToFileDescriptor", &google::protobuf::Message::SerializeToFileDescriptor);')
    source.append('                binder.def("SerializePartialToFileDescriptor", &google::protobuf::Message::SerializePartialToFileDescriptor);')
    source.append('                binder.def("SerializeToOstream", &google::protobuf::Message::SerializeToOstream);')
    source.append('                binder.def("SerializePartialToOstream", &google::protobuf::Message::SerializePartialToOstream);')
    source.append('                binder.def("GetTypeName", &google::protobuf::Message::GetTypeName);')
    source.append('                binder.def("Clear", &google::protobuf::Message::Clear);')
    source.append('                binder.def("IsInitialized", &google::protobuf::Message::IsInitialized);')
    source.append('                binder.def("CheckTypeAndMergeFrom", &google::protobuf::Message::CheckTypeAndMergeFrom);')
    source.append('                binder.def("MergePartialFromCodedStream", &google::protobuf::Message::MergePartialFromCodedStream);')
    source.append('                binder.def("ByteSize", &google::protobuf::Message::ByteSize);')
    source.append('                binder.def("SerializeWithCachedSizes", &google::protobuf::Message::SerializeWithCachedSizes);')
    source.append('            }') # protobuf scope
    source.append('         }') # google scope
    source.append('    }') # main scope    

    scope = '::'.join(package)

    for name in generated_names:
        source.append('    %s::make_%s_bindings();' % (scope, name))
    source.append('}')

    return source

def main():
    # Switch to binary mode
    msvcrt.setmode (sys.stdin.fileno(), os.O_BINARY)
    msvcrt.setmode (sys.stdout.fileno(), os.O_BINARY)
    cg_req = plugin_pb2.CodeGeneratorRequest()
    cg_req.ParseFromString(sys.stdin.read())

    #print >> sys.stderr, cg_req

    files_to_generate = set(cg_req.file_to_generate)

    cg_resp = plugin_pb2.CodeGeneratorResponse()

    for proto_file in cg_req.proto_file:

        if proto_file.name not in files_to_generate:
            continue

        # base name of input proto file
        name = proto_file.name.split('.')[0]

        # package namespace components
        package = proto_file.package.split('.') if proto_file.package else []

        # expected location of generated cpp header
        cpp_header = package + ['%s.pb.h' % name]
        cpp_header = os.path.join(*cpp_header)

        # path template for generated Message / Enum sources
        msg_path = '/'.join([name, '%s.cpp'])

        generated_names = []

        enum_queue = [(i.name, i) for i in proto_file.enum_type]
        msg_queue  = [(i.name, i) for i in proto_file.message_type]

        # Enumerate messages
        while msg_queue:

            msg_full_name, msg_type = msg_queue.pop(0)

            ofile = cg_resp.file.add()
            ofile.name = msg_path % msg_full_name

            #print >> sys.stderr, ofile.name

            ofile.content = '\n'.join(
                generate_message(msg_full_name, msg_type, cpp_header, package))

            generated_names.append(msg_full_name)

            for nested_msg in msg_type.nested_type:
                msg_queue.append(('%s_%s' % (
                    msg_full_name, nested_msg.name), nested_msg))

            for nested_enum in msg_type.enum_type:
                enum_queue.append(('%s_%s' % (
                    msg_full_name, nested_enum.name), nested_enum))

        # Enumerate enumerations
        while enum_queue:

            enum_full_name, enum_type = enum_queue.pop(0)

            ofile = cg_resp.file.add()
            ofile.name = msg_path % enum_full_name

            #print >> sys.stderr, ofile.name

            ofile.content = '\n'.join(
                generate_enum(enum_full_name, enum_type, cpp_header, package))

            generated_names.append(enum_full_name)

        # Generate top-level module
        ofile = cg_resp.file.add()
        ofile.name = '_%s.cpp' % name

        ofile.content = '\n'.join(
            generate_module(name, generated_names, package))

    sys.stdout.write(cg_resp.SerializeToString())

MSG_HEAD = """
#include <boost/python.hpp>
#include <google/protobuf/text_format.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "%s"

namespace bpl = boost::python;
"""[1:]

ENUM_DEFINITION = """

namespace bpl = boost::python;

void make_%(enum_name)s_bindings()
{
    bpl::enum_<%(enum_name)s> binder("%(enum_name)s");
"""[1:-1]

ENUM_VALUE = """
    binder.value("%(value_name)s", %(value_name)s);
"""[1:-1]

MSG_DEFINITION = """
bool py_ParseFromBytes(%(msg_name)s & msg, const bpl::str & s)
{
    PyObject *pUTF8 = PyUnicode_AsUTF8String(s.ptr());
    google::protobuf::io::ArrayInputStream input(
        PyBytes_AsString(pUTF8), PyBytes_Size(pUTF8));
    Py_XDECREF(pUTF8);
    return msg.ParseFromZeroCopyStream(&input);
}

std::string py_SerializeToBytes(%(msg_name)s & msg)
{
    std::string result;
    msg.SerializeToString(&result);
    return result;
}

bool py_ParseFromText(%(msg_name)s & msg, const bpl::str & s)
{
    PyObject *pUTF8 = PyUnicode_AsUTF8String(s.ptr());
    google::protobuf::io::ArrayInputStream input(
        PyBytes_AsString(pUTF8), PyBytes_Size(pUTF8));
    Py_XDECREF(pUTF8);
    
    return google::protobuf::TextFormat::Parse(&input, &msg);
}

std::string py_SerializeToText(%(msg_name)s & msg)
{
    if(!msg.IsInitialized())
    {
        throw std::runtime_error("%(msg_name)s isn't initialized");
    }

    std::string result;
    google::protobuf::TextFormat::PrintToString(msg, &result);
    return result;
}

void make_%(msg_name)s_bindings()
{
    bpl::class_<%(msg_name)s, bpl::bases<google::protobuf::Message>> binder("%(msg_name)s");
    binder.def(bpl::init<const %(msg_name)s &>());

    void (%(msg_name)s::*copy_from_ptr)(const %(msg_name)s &) = \\
        &%(msg_name)s::CopyFrom;

    // TODO(johng): My preference would be for __slots__-like behavior
    //   on wrapped Message classes. The mechanics of boost::python
    //   make this difficult.
    //
    // To prevent typos in assignment from causing inadvertant errors
    //  where fields are silently added onto the Python message instance,
    //  I've elected to implement the C++ set_$(field) api for assigment.

    // Common message methods
    binder.def("__str__", &%(msg_name)s::DebugString);
    binder.def("IsInitialized", &%(msg_name)s::IsInitialized);
    binder.def("Clear", &%(msg_name)s::Clear);
    binder.def("CopyFrom", copy_from_ptr);
    binder.def("ParseFromBytes", &py_ParseFromBytes);
    binder.def("SerializeToBytes", &py_SerializeToBytes);
    binder.def("ParseFromText", &py_ParseFromText);
    binder.def("SerializeToText", &py_SerializeToText);
    binder.def("ByteSize", &%(msg_name)s::ByteSize);

"""[1:-1]


type_map = {
    FieldDesc.TYPE_DOUBLE:   'basic',
    FieldDesc.TYPE_FLOAT:    'basic',
    FieldDesc.TYPE_INT64:    'basic',
    FieldDesc.TYPE_UINT64:   'basic',
    FieldDesc.TYPE_INT32:    'basic',
    FieldDesc.TYPE_FIXED64:  'basic',
    FieldDesc.TYPE_FIXED32:  'basic',
    FieldDesc.TYPE_BOOL:     'basic',
    FieldDesc.TYPE_UINT32:   'basic',
    FieldDesc.TYPE_SFIXED32: 'basic',
    FieldDesc.TYPE_SFIXED64: 'basic',
    FieldDesc.TYPE_SINT32:   'basic',
    FieldDesc.TYPE_SINT64:   'basic',
    FieldDesc.TYPE_ENUM:     'basic',

    FieldDesc.TYPE_STRING:   'str',
    FieldDesc.TYPE_BYTES:    'str',

    FieldDesc.TYPE_MESSAGE:  'message',
}

cpp_type_map = {
    FieldDesc.TYPE_DOUBLE:   'double',
    FieldDesc.TYPE_FLOAT:    'float',
    FieldDesc.TYPE_INT64:    '::google::protobuf::int64',
    FieldDesc.TYPE_UINT64:   '::google::protobuf::uint64',
    FieldDesc.TYPE_INT32:    '::google::protobuf::int32',
    FieldDesc.TYPE_FIXED64:  '::google::protobuf::uint64',
    FieldDesc.TYPE_FIXED32:  '::google::protobuf::uint32',
    FieldDesc.TYPE_BOOL:     'bool',
    FieldDesc.TYPE_UINT32:   '::google::protobuf::uint32',
    FieldDesc.TYPE_SFIXED32: '::google::protobuf::int32',
    FieldDesc.TYPE_SFIXED64: '::google::protobuf::int64',
    FieldDesc.TYPE_SINT32:   '::google::protobuf::int32',
    FieldDesc.TYPE_SINT64:   '::google::protobuf::int64',

    FieldDesc.TYPE_STRING:   'const std::string &',
    FieldDesc.TYPE_BYTES:    'const std::string &',
}

method_templates = {}

method_templates[(FieldDesc.LABEL_OPTIONAL, 'message')] = \
method_templates[(FieldDesc.LABEL_REQUIRED, 'message')] = """
bpl::object py_%(field_name)s(%(msg_name)s & msg)
{
    if(msg.has_%(field_name)s())
    {
        bpl::reference_existing_object::apply<
            const %(cpp_type)s &>::type convert;

        return bpl::object(bpl::handle<>(convert(msg.%(field_name)s())));
    }
    // return None
    return bpl::object();
}
""" 

## REPEATED fields
#
#   These are a bit tricker. For pythonic behavior, we use a proxy class
#   for 'capturing' the field post attribute-lookup, which implements
#   expected list-like behaviors. Another proxy class is used to implement
#   the iterator protocol.

repeated_method_common = """ 

class %(msg_name)s_%(field_name)s_proxy
{
public:

    static %(msg_name)s_%(field_name)s_proxy proxy(%(msg_name)s & msg)
    { return %(msg_name)s_%(field_name)s_proxy(msg); }

    %(msg_name)s_%(field_name)s_proxy(%(msg_name)s & msg)
      : _m(msg) { }

    int len()
    { return _m.%(field_name)s_size(); }

    %(cpp_type)s getitem(int ind);

    void setitem(unsigned ind, %(cpp_type)s);

    bpl::object iter();

    void SwapElements(int ind1, int ind2)
    { _m.mutable_%(field_name)s()->SwapElements(ind1, ind2); }

private:

    %(msg_name)s & _m; 
};

class %(msg_name)s_%(field_name)s_iter
{
public:

    %(msg_name)s_%(field_name)s_iter(%(msg_name)s & msg)
     : _p(msg), _ind(0) { }

    %(cpp_type)s next()
    {
        if(_ind == _p.len())
        {
            PyErr_SetObject(PyExc_StopIteration, Py_None);
            boost::python::throw_error_already_set();
        }
        return _p.getitem(_ind++);
    }

private:

    %(msg_name)s_%(field_name)s_proxy _p;
    int _ind;
};

bpl::object %(msg_name)s_%(field_name)s_proxy::iter()
{ return bpl::object(%(msg_name)s_%(field_name)s_iter(_m)); }

"""

# Core-type implementations of setitem, getitem, & add_%(field_name)s
method_templates[(FieldDesc.LABEL_REPEATED, 'basic')] = \
method_templates[(FieldDesc.LABEL_REPEATED, 'str')] = \
    repeated_method_common + \
"""
%(cpp_type)s %(msg_name)s_%(field_name)s_proxy::getitem(int ind)
{
    if(ind < 0 || ind >= len())
        throw std::range_error("%(field_name)s index range error");

    return _m.%(field_name)s(ind);
}

void %(msg_name)s_%(field_name)s_proxy::setitem(unsigned ind, %(cpp_type)s val)
{ _m.set_%(field_name)s(ind, val); }

void py_add_%(field_name)s(%(msg_name)s & m, %(cpp_type)s val)
{ m.add_%(field_name)s(val); }

"""

method_templates[(FieldDesc.LABEL_REPEATED, 'message')] = \
    repeated_method_common + \
"""
%(cpp_type)s
%(msg_name)s_%(field_name)s_proxy::getitem(int ind)
{
    if(ind < 0 || ind >= len())
        throw std::range_error("%(field_name)s index range error");

    return _m.mutable_%(field_name)s(ind);
}

%(cpp_type)s py_add_%(field_name)s(
    %(msg_name)s & m)
{ return m.add_%(field_name)s(); }

"""

binding_templates = {}

# BASIC types

binding_templates[(FieldDesc.LABEL_OPTIONAL, 'basic')] = """
    binder.def("has_%(field_name)s", &%(msg_name)s::has_%(field_name)s);
    binder.def("set_%(field_name)s", &%(msg_name)s::set_%(field_name)s);
    binder.def("clear_%(field_name)s", &%(msg_name)s::clear_%(field_name)s);
    binder.add_property("%(field_name)s", &%(msg_name)s::%(field_name)s);
"""[1:-1]

binding_templates[(FieldDesc.LABEL_REQUIRED, 'basic')] = """
    binder.def("set_%(field_name)s", &%(msg_name)s::set_%(field_name)s);
    binder.add_property("%(field_name)s", &%(msg_name)s::%(field_name)s);
"""[1:-1]

# STRING types

binding_templates[(FieldDesc.LABEL_OPTIONAL, 'str')] = """
    void (%(msg_name)s::*set_%(field_name)s_ptr)(const std::string &) = \\
        &%(msg_name)s::set_%(field_name)s;

    binder.def("set_%(field_name)s", set_%(field_name)s_ptr);
    binder.def("has_%(field_name)s", &%(msg_name)s::has_%(field_name)s);
    binder.def("clear_%(field_name)s", &%(msg_name)s::clear_%(field_name)s);
    binder.add_property("%(field_name)s",
        bpl::make_function(&%(msg_name)s::%(field_name)s,
            bpl::return_value_policy<bpl::copy_const_reference>()));
"""[:-1]

binding_templates[(FieldDesc.LABEL_REQUIRED, 'str')] = """
    void (%(msg_name)s::*set_%(field_name)s_ptr)(const std::string &) = \\
        &%(msg_name)s::set_%(field_name)s;

    binder.def("set_%(field_name)s", set_%(field_name)s_ptr);
    binder.add_property("%(field_name)s",
        bpl::make_function(&%(msg_name)s::%(field_name)s,
            bpl::return_value_policy<bpl::copy_const_reference>()));
"""[:-1]

# MESSAGE type

binding_templates[(FieldDesc.LABEL_OPTIONAL, 'message')] = """
    binder.def("has_%(field_name)s", &%(msg_name)s::has_%(field_name)s);
    binder.add_property("%(field_name)s", &py_%(field_name)s);
    binder.def("mutable_%(field_name)s", &%(msg_name)s::mutable_%(field_name)s,
        bpl::return_value_policy<bpl::reference_existing_object>());
    binder.def("clear_%(field_name)s", &%(msg_name)s::clear_%(field_name)s);
"""[1:-1]

binding_templates[(FieldDesc.LABEL_REQUIRED, 'message')] = """
    binder.add_property("%(field_name)s", &py_%(field_name)s);
    binder.def("mutable_%(field_name)s", &%(msg_name)s::mutable_%(field_name)s,
        bpl::return_value_policy<bpl::reference_existing_object>());
"""[1:-1]

# REPEATED labels

repeated_binding_common = """

    binder.def("clear_%(field_name)s", &%(msg_name)s::clear_%(field_name)s);
    binder.add_property("%(field_name)s",
        &%(msg_name)s_%(field_name)s_proxy::proxy);

    bpl::class_<%(msg_name)s_%(field_name)s_iter> %(field_name)s_iter_binder(
        "%(msg_name)s_%(field_name)s_iter", bpl::no_init);

    bpl::class_<%(msg_name)s_%(field_name)s_proxy> %(field_name)s_proxy_binder(
        "%(msg_name)s_%(field_name)s_proxy", bpl::no_init);
    %(field_name)s_proxy_binder.def("__len__",
        &%(msg_name)s_%(field_name)s_proxy::len);
    %(field_name)s_proxy_binder.def("__iter__",
        &%(msg_name)s_%(field_name)s_proxy::iter);
    %(field_name)s_proxy_binder.def("SwapElements",
        &%(msg_name)s_%(field_name)s_proxy::SwapElements);

"""

binding_templates[(FieldDesc.LABEL_REPEATED, 'basic')] = \
    repeated_binding_common + \
"""
    %(field_name)s_proxy_binder.def("__getitem__",
        &%(msg_name)s_%(field_name)s_proxy::getitem);

    %(field_name)s_proxy_binder.def("__setitem__",
        &%(msg_name)s_%(field_name)s_proxy::setitem);

    binder.def("add_%(field_name)s", &py_add_%(field_name)s);

    %(field_name)s_iter_binder.def("next",
        &%(msg_name)s_%(field_name)s_iter::next);

"""

binding_templates[(FieldDesc.LABEL_REPEATED, 'str')] = \
    repeated_binding_common + \
"""
    %(field_name)s_proxy_binder.def("__getitem__",
        &%(msg_name)s_%(field_name)s_proxy::getitem,
        bpl::return_value_policy<bpl::copy_const_reference>());

    %(field_name)s_proxy_binder.def("__setitem__",
        &%(msg_name)s_%(field_name)s_proxy::setitem);

    binder.def("add_%(field_name)s", &py_add_%(field_name)s);

    %(field_name)s_iter_binder.def("next",
        &%(msg_name)s_%(field_name)s_iter::next,
        bpl::return_value_policy<bpl::copy_const_reference>());
"""

binding_templates[(FieldDesc.LABEL_REPEATED, 'message')] = \
    repeated_binding_common + \
"""
    %(field_name)s_proxy_binder.def("__getitem__",
        &%(msg_name)s_%(field_name)s_proxy::getitem,
        bpl::return_value_policy<bpl::reference_existing_object>());

    // __setitem__ not defined

    binder.def("add_%(field_name)s", &py_add_%(field_name)s,
        bpl::return_value_policy<bpl::reference_existing_object>());

    %(field_name)s_iter_binder.def("next",
        &%(msg_name)s_%(field_name)s_iter::next,
        bpl::return_value_policy<bpl::reference_existing_object>());
"""

