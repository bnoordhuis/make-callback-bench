"use strict"

const pp = console.log
const {nodeMakeCallback, v8FunctionCall} = require("./build/Release/binding")

function bench(n, f, ...xs) {
    const a = Date.now()
    f.call(null, n, {}, () => {}, ...xs)
    const b = Date.now()
    return (1e6 * (b - a)) / n // ms -> ns/op
}

const a = bench(7e6, nodeMakeCallback)
const b = bench(7e6, v8FunctionCall)
pp(a.toFixed(1), "\t", "node::MakeCallback")
pp(b.toFixed(1), "\t", "v8::Function::Call")
