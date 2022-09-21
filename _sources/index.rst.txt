.. Licensecxx documentation master file, created by
   sphinx-quickstart on Wed Sep 21 12:41:19 2022.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Licensecxx's documentation!
======================================

.. |Build & Test Status Ubuntu| image:: https://github.com/felixjulianheitmann/licensecxx/actions/workflows/build-test-ubuntu.yml/badge.svg

.. |Build & Test Status Windows| image:: https://github.com/felixjulianheitmann/licensecxx/actions/workflows/build-test-windows.yml/badge.svg

.. |Build & Test Status Mac| image:: https://github.com/felixjulianheitmann/licensecxx/actions/workflows/build-test-mac.yml/badge.svg


.. toctree::
   :maxdepth: 2
   :caption: Contents:

   Identifiers
   How to use licensecxx<How-to-use-it>
   api/library_root

Intro
-----

*Copy protection library targeting Linux, Windows and Mac* (currently only Linux supported)

This project is inspired by the licensecc_ project.

Protect your software by generating and checking against license files. Create a set of key value pairs as content for your file and sign them with a RSA private key. The licensed software can then read this license file and verify the signature with the respective public key.

Additionally to providing key value pairs, you can include hash values of machine or user dependant data. Features such as name of the operating system, the machine hostname, the maximum CPU frequency can be hashed and included in the license. This enables machine/user specific license distribution.

Licensecxx provides a modern C++ interface with the main focus on ease of use and minimal impact on the software that it's protecting.

Due to the early development stage, licensecxx currently only supports linux hosts. Most of the software should be os-agnostic, but hardware/os identifiers rely on the operatings system they run on. One of the primary goals will be to enable cross-platform support, as soon as I can get my hands on a Windows/Mac to test things on.

A list of the collectable environment information is given at :ref:`identifiers_doc`

This repository is still under development. If you have experience errors or bugs, please file an issue on the GitHub issues page. If you want to contribute, pull requests are very welcome.


Why use it
----------

This library makes it easy to generate any form of license protection for your software.

You can freely include any form of verifyable information, that will be signed and limit the use of your software.

When I needed this feature myself, I could not find any open source tools that would help me with it, except for licensecc_.
The problem I had with their toolchain was that it requires the user project to be tailored around the licensecc project.
I would have much rather preferred a simple library that allows me to generate signatures through a private keys and verify these with a corresponding public key.

This is the idea that spawned licensecxx: Provide a modern C++ library interface, that allows signing and verifying key-value pairs,
which is easy to use and has minimal impact on the including user project.

Similarly, to it's spiritual predecessor, it provides the option to collect system information such as CPU-Name, OS-Name, Hostname, etc, hash them,
and incorporate them into the license. A full list of collectable info can be found at :ref:`identifiers_doc`.



Indices and tables
------------------

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

.. _licensecc: https://github.com/open-license-manager/licensecc
