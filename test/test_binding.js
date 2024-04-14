const binding = require("../lib/binding.js");
const fs = require('fs');
const result = binding.listProcesses();
console.log(result);
