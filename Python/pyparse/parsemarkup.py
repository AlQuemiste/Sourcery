# Input
input_source = """
Heading
=======

Sub-heading
-----------

Paragraphs are separated
by a blank line.

Two spaces at the end of a line  
produces a line break.

Text attributes _italic_, 
**bold**, `monospace`.

Horizontal rule:

---

Strikethrough:
~~strikethrough~~


Bullet list:

  * apples
  * oranges
  * pears

Numbered list:

  1. lather
  2. rinse
  3. repeat


An [example](http://example.com).

![Image](Icon-pictures.png "icon")

> Markdown uses email-style 
> characters for blockquoting.

Inline <abbr title="Hypertext Markup Language">HTML</abbr> is supported.

"""
import re

# operator codes
PAR = 0
HEADER = 1
SUBHEADER = 2
BOLDCASE = 3
BULLETLIST = 4

tokens_ = ((r"={3,}", HEADER),
          (r"-{3,}", SUBHEADER),
          (r"\*\*(.+?)\*\*", BOLDCASE),
          (r"\*\s*(.+)", BULLETLIST)
         )

tokens = [(re.compile(r), code) for r, code in tokens_]

def parse(string):
    """ Parse a markup string into a tokenized list """
    
    pos = 0  # current position on the string
    ln = len(string)
    tokenized = list()
    _i = 1
    while pos < ln and _i < 5:
        _i += 1
        substr = string[pos:]

        if not substr:
            break
        matched = False
        # try each token in order
        for token, opcode in tokens:
            match = token.match(substr)
            if match:
                matched = True
                # pos1 = match.pos
                pos2 = match.endpos
                if match.groups():
                    tokenized.extend((match.group(1), opcode))
                else:
                    tokenized.append(opcode)

                pos = pos2 + 1

                break
            
        if not matched:
            tokenized.append(substr)
            pos += len(substr)
    return tokenized


def apply_op(opcode, args):
    """ Apply operation to the arguments """
    argstr = ''.join(args)
    if opcode == PAR:
        html = "<p>" + argstr + "</p>"
    elif opcode == HEADER:
        html = "<h1>" + argstr + "</h1>"
    elif opcode == SUBHEADER:
        html = "<h2>" + argstr + "</h2>"
    elif opcode == BOLDCASE:
        html = "<strong>" + argstr + "</strong>"
    elif opcode == BULLETLIST:
        items = " ".join("<li>" + it + "</li>" for it in args)
        html = "<ul>" + items + "</ul>"
    return html

def convert_tokenized_to_HTML(tokenized):
    html = ""
    ln = len(tokenized)
    pos = 0
    while pos < ln:
        # collect arguments for the operation
        args = []
        while pos < ln and not isinstance(tokenized[pos], int):
            args.append(tokenized[pos])
            pos += 1
        if pos < ln:
            opcode = tokenized[pos]
        else:
            opcode = PAR
        pos += 1
        html_str = apply_op(opcode, args)
        print(f"html_str = {html_str}")
        html += html_str

    return html
        

def convert_to_HTML(string):
    tokenized = parse(string.strip())
    print(f"tokenized = {tokenized}")
    html = convert_tokenized_to_HTML(tokenized)
    return html


def test_convert_to_HTML():
    cases = ("**bold**", "Heading\n=======", "Sub-heading\n-----------")
    for case in cases:
        html = convert_to_HTML(case)
        print(f"markup = '{case}'")
        print(f"html = '{html}'")


test_convert_to_HTML()
