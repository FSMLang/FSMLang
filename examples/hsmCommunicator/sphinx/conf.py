# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information
import sys
import os

project = 'hsmCommunicator'
copyright = '2025, FSMLang Maintainers'
author = 'FSMLang Maintainers'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
	'sphinxcontrib.plantuml',
	'sphinxcontrib.svgbob',
	'rst2pdf.pdfbuilder',
	'breathe'
]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# -- Options for breathe output ----------------------------------------------
breathe_projects = {
	"pod_os": "./_build/doxygen_out/xml/"
}
breath_default_project = "hsmCommunicator"

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_theme = 'agogo'
html_theme_options = {
	'pagewidth': '85%'
	, 'documentwidth': '75%'
	, 'sidebarwidth': '15%'
}
html_static_path = ['_static']
html_css_files = [
	 'fsmlangweb.css'
	, 'fsmlang.css'
]


# -- Options for PLANTUML output -------------------------------------------------
# https://github.com/sphinx-contrib/plantuml

#plantuml_output_format = "svg_img"

def setup(sphinx):
	sys.path.insert(0, os.path.abspath('./_pygments'))
	from fsmlang import FSMLangLexer
	sphinx.add_lexer("fsmlang", FSMLangLexer)

