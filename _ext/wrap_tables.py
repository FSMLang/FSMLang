from docutils import nodes

MARKER_CLASS = "scrollable-target"   # class added in rst
WRAPPER_CLASS = "scrollable"         # class for the div wrapper

def wrap_marked_tables(app, doctree, docname):
    for table in list(doctree.findall(nodes.table)):
        classes = table.get("classes", [])
        if MARKER_CLASS not in classes:
            continue

        parent = table.parent
        if parent is None:
            continue

        # Avoid double-wrapping
        if isinstance(parent, nodes.container) and WRAPPER_CLASS in parent.get("classes", []):
            continue

        wrapper = nodes.container(classes=[WRAPPER_CLASS])
        table.replace_self(wrapper)
        wrapper += table

        # Optional: remove marker class from final HTML output
        if MARKER_CLASS in classes:
            classes.remove(MARKER_CLASS)
            table["classes"] = classes

def setup(app):
    app.connect("doctree-resolved", wrap_marked_tables)
    return {
        "version": "1.0",
        "parallel_read_safe": True,
        "parallel_write_safe": True,
    }
