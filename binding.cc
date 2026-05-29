// Copyright (c) 2026, nxtedition
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
// WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
// ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
// WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
// ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
// OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

#include "node.h"
#include <vector>

namespace {

void Throw(v8::Isolate* isolate, const char* message)
{
    auto arg = v8::String::NewFromUtf8(isolate, message).ToLocalChecked();
    auto exc = v8::Exception::TypeError(arg);
    isolate->ThrowException(exc);
}

template <bool makeCallback>
void Bench(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    auto isolate = args.GetIsolate();
    auto context = isolate->GetCurrentContext();
    if (!args[0]->IsUint32()) return Throw(isolate, "not an uint32");
    if (!args[1]->IsObject()) return Throw(isolate, "not an object");
    if (!args[2]->IsFunction()) return Throw(isolate, "not a function");
    uint32_t count = args[0].As<v8::Uint32>()->Value();
    auto recv = args[1].As<v8::Object>();
    auto callback = args[2].As<v8::Function>();
    std::vector<v8::Local<v8::Value>> argv(args.Length() - 3);
    for (int i = 3; i < args.Length(); i++) argv.push_back(args[i]);
    for (uint32_t i = 0, step = 4096; i < count; i += step) {
        // avoid bloating the outer HandleScope but also don't create one
        // on every iteration of the loop (amortize)
        v8::HandleScope handle_scope(isolate);
        for (uint32_t j = 0; j < step; j++) {
            if (makeCallback) {
                node::MakeCallback(isolate, recv, callback, argv.size(),
                                   argv.data(), node::async_context{});
            } else {
                v8::Local<v8::Value> unused;
                if (callback->Call(isolate, context, recv, argv.size(),
                                   argv.data()).ToLocal(&unused)) {
                }
            }
        }
    }
}

void Initialize(v8::Local<v8::Object> exports,
                v8::Local<v8::Value> module_,
                v8::Local<v8::Context> context,
                void* unused)
{
    auto isolate = v8::Isolate::GetCurrent();
    auto undefined = v8::Undefined(isolate);
    {
        auto key = v8::String::NewFromUtf8Literal(isolate, "nodeMakeCallback");
        auto value = v8::Function::New(context, Bench<true>, undefined, /*length*/3).ToLocalChecked();
        exports->Set(context, key, value).Check();
    }
    {
        auto key = v8::String::NewFromUtf8Literal(isolate, "v8FunctionCall");
        auto value = v8::Function::New(context, Bench<false>, undefined, /*length*/3).ToLocalChecked();
        exports->Set(context, key, value).Check();
    }
}

NODE_MODULE_CONTEXT_AWARE(NODE_GYP_MODULE_NAME, Initialize)

}
