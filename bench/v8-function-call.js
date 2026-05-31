"use strict"
const {bench, v8FunctionCall} = require("./util")
bench("v8::Function::Call", v8FunctionCall)
