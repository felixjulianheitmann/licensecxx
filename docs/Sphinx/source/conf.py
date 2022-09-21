# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Licensecxx'
copyright = '2022, Felix Heitmann'
author = 'Felix Heitmann'
release = 'latest'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'breathe',
    'exhale'
]

templates_path = ['_templates']
exclude_patterns = []

language = 'en'

# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'alabaster'
html_static_path = ['_static']

# -- Exhale configuration --------------------------------------------------
breathe_projects = {
    "Licensecxx": "./../../Doxygen/gen_docs/xml"
}
breathe_default_project = "Licensecxx"

exhale_args = {
    "containmentFolder":    "./api",
    "rootFileName":         "library_root.rst",
    "rootFileTitle":        "Library API",
    "doxygenStripFromPath": "..",
    "createTreeView":       True,
}

primary_domain = 'cpp'
highlight_language = 'cpp'

# -- Theme configuration --------------------------------------------------

html_theme = 'sphinx_material'

html_sidebars = {
    "**": ["globaltoc.html"]
}

html_theme_options = {
    'nav_title': 'Licensecxx',
    'nav_links': [
        {
            'href':     'Intro',
            'title':    'Intro',
            'internal': True,
        },
        {
            'href':     'Identifiers',
            'title':    'Identifiers',
            'internal': True,
        },
        {
            'href':     'How-to-use-it',
            'title':    'How to use licensecxx',
            'internal': True,
        },
    ],

    # 'version_dropdown': True,
    # 'version_info': {
    #     'Release': '.',
    #     '0.1.0': '0.1.0',
    # },

    # Specify a base_url used to generate sitemap.xml. If not
    # specified, then no sitemap will be built.
    'base_url': 'https://felixjulianheitmann.github.io/licensecxx',

    # Set the color and the accent color
    'color_primary': 'blue',
    'color_accent': 'light-blue',

    # Set the repo location to get a badge with stats
    'repo_url': 'https://github.com/felixjulianheitmann/licensecxx/',
    'repo_name': 'Licensecxx',

    # Visible levels of the global TOC; -1 means unlimited
    'globaltoc_depth': 3,
    # If False, expand all TOC entries
    'globaltoc_collapse': False,
    # If True, show hidden TOC entries
    'globaltoc_includehidden': False,
}
