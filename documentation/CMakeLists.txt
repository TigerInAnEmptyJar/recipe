find_package(Doxygen
             REQUIRED dot
             OPTIONAL_COMPONENTS mscgen dia)

doxygen_add_docs(documentation
               ./
               documentation
)
