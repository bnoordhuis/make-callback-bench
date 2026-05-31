"use strict"

const N = 7e6
const pp = console.log
const {nodeMakeCallback, v8FunctionCall} = require("../build/Release/binding")

function bench(name, f, ...xs) {
    const a = Date.now()
    f.call(null, N, {}, () => {}, ...xs)
    const b = Date.now()
    const t = (1e6 * (b - a)) / N // ms -> ns/op
    pp(t.toFixed(1), "ns/op\t", name)
}

module.exports = {bench, nodeMakeCallback, pp, v8FunctionCall}
