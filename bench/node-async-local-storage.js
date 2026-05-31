"use strict"
const {bench, nodeMakeCallback} = require("./util")
const {AsyncLocalStorage} = require("node:async_hooks")
const asyncLocalStorage = new AsyncLocalStorage()

bench("node::MakeCallback + AsyncLocalStorage",
      AsyncLocalStorage.bind(nodeMakeCallback))
