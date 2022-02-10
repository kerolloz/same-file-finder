const crypto = require("crypto");
const fs = require("fs");
const path = require("path");

class HashStorage {
  constructor() {
    this.storage = {};
  }

  addHash(hash, filePath) {
    if (this.storage[hash]) {
      this.storage[hash].push(filePath);
      return;
    }
    this.storage[hash] = [filePath];
  }

  getSimilar() {
    const result = {};
    for (const [key, value] of Object.entries(this.storage))
      if (value.length > 1) result[key] = value;

    return Object.values(result);
  }
}

const getFileHash = (filePath) => {
  const fileContents = fs.readFileSync(filePath);
  const hash = crypto.createHash("md5");
  const data = hash.update(fileContents, "utf-8");
  return data.digest("hex");
};

const readDir = (dir) => {
  let contents = fs.readdirSync(dir).map((x) => path.join(dir, x));
  for (let x of contents) {
    if (fs.lstatSync(x).isDirectory()) contents = contents.concat(readDir(x));
  }
  return contents.filter((f) => fs.lstatSync(f).isFile());
};

(() => {
  const dir = process.argv[2];
  if (dir && fs.existsSync(dir) && fs.lstatSync(dir).isDirectory()) {
    const hashStorage = new HashStorage();
    for (const file of readDir(dir)) {
      hashStorage.addHash(getFileHash(file), file);
    }
    console.log(hashStorage.getSimilar());
    process.exit(0);
  }
  console.error("Please provide a valid directory path");
  process.exit(-1);
})();
