"use strict"

run("v8-function-call")
run("node-make-callback")
run("node-async-hooks")
run("node-async-local-storage")

function run(name) {
    const {spawnSync} = require("node:child_process")
    const args = [`./bench/${name}.js`]
    spawnSync(process.execPath, args, {stdio: "inherit"})
}
