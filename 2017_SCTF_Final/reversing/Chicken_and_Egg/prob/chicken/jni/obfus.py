import re

with open('egg_tmpl.c', 'r') as f:
    lines = f.read().splitlines()
    buf_idx = 0
    i = 0
    while i < len(lines):
        m = re.search(r'XXOBFUS\("([^"]+)"\)', lines[i])
        if not m:
            i += 1
            continue

        expr = m.group(0)
        pay = m.group(1)

        buf = 'buf_{}'.format(buf_idx)
        buf_idx += 1
        buf_init = 'char {}[{}];'.format(buf, len(pay)+1)
        for j in range(len(pay)):
            buf_init += '\n{}[{}] = {};'.format(buf, j, ord(pay[j])^(j+1));
        buf_init += '\n{}[{}] = 0;'.format(buf, len(pay))
        decode_call = 'DECODE_STR({}, {});'.format(buf, len(pay));

        lines[i] = lines[i].replace(expr, buf)
        lines.insert(i, buf_init)
        lines.insert(i+1, decode_call)

        i += 2

with open('egg.c', 'w') as f:
    f.write('\n'.join(lines))
