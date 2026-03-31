include(GNUInstallDirs)
install(
    FILES
        CLA_create.3
        CLA_destroy.3
        CLA_get_value_optional.3
        kwargs_get_positional_value.3
        kwargs_get_required_value.3
        kwargs_has_flag.3
        kwargs_has_optional.3
        kwargs_print_classifications.3
        kwargs_requires_help.3
        CLA.3

    DESTINATION ${CMAKE_INSTALL_MANDIR}/man3     # I'd prefer to use TYPE instead of DESTINATION but it seems I need the man3 subdirectory
)
