#!/usr/bin/env zx

function addHeaderDefine(dirname, filename) {
  const fullpath = path.join(dirname, filename);
  const content = fs.readFileSync(fullpath, 'utf8');
  const lines = content.split("\n");

  echo(`Checking ${chalk.yellow(fullpath)}...`);

  if (!/^#ifndef/.test(lines[0])) {
    const name = `${filename.toUpperCase()}`.replace(/\./, '_');

    lines.unshift(`#ifndef ${name} 
#define ${name}
`);
    lines.push(`
#endif
`)

    echo(`Writing ${chalk.yellow(fullpath)}...`);
    fs.writeFileSync(fullpath, lines.join("\n"), "utf8")
  }
}


function scanDir(dir) {
  fs.readdirSync(dir).map(child => {
    const fullpath = path.join(dir, child);

    if (/\.h$/.test(child)) {
      addHeaderDefine(dir, child);
    } else if (fs.statSync(fullpath).isDirectory()) {
      scanDir(fullpath);
    }
  })
}

scanDir(path.join('src'));
