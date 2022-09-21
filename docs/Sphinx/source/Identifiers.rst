.. _identifiers_doc:

Identifiers
===========

.. toctree::
   :maxdepth: 2
   :caption: Contents:

Licensecxx allows to gather information about the system currently running the software. To use those features, link against the ``lcxx::identifiers`` target in your CMake configuration.

This will provide the following headers:
- ``lcxx/identifiers/hardware.hpp``
- ``lcxx/identifiers/os.hpp``

There two primary functions are given (arguments omitted):
- ``lcxx::identifiers::<header-name>()`` (e.g. ``lcxx::identifiers::hardware()``)
- ``lcxx::identifiers::verify()``

The type of information that will be collected depends on the chosen strategy. Sticking to the ``hardware`` example, the enum ``hw_ident_strat`` defines which information will be incorporated into the hardware identifier.

Passing the value ``hw_ident_strat::all`` will try to collect as much info about the machine and generate a single ``identifier`` object from that. This object contains a string containing all collect info and a hash-digest of that string.

This hash (or the respective string) can be included as a value into the license.

The ``lcxx::identifiers::verify`` function takes a ``hw_ident_strat`` and a hash value and will check whether those match, given the current underlying hardware.

Multiple identification strategies can be combined by using the ``operator|``. Using ``hw_ident_strat::cpu_n_cores | hw_ident_strat::cpu_model_name`` will only collect the number of 
available CPU cores and the CPU model name.

The following code snipped would take a license object and push in the hardware identifier consisting of no. of CPU cores and its model name.

.. code-block:: c++

    #include <lcxx/lcxx.hpp>
    #include <lcxx/identifiers/hardware.hpp>

    int main()
    {
        using namespace lcxx;
        using hw_strat = identifiers::hw_ident_strat;

        license lic;
        auto [hash, text] = identifiers::hardware( hw_strat::cpu_n_cores | hw_strat::cpu_model_name );
        lic.push_content( "hardware", hash );

        // ... rest of the software
    }

Hardware
--------

The hardware header allows to collect information about the systems hardware, if the current user has the privileges to access that information.

