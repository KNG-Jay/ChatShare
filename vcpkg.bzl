def _get_packages_impl(repository_ctx): 
    repository_ctx.symlink(repository_ctx.attr.path, "vcpkg_installed/arm64-linux")

get_packages = repository_rule(
    implementation = _get_packages_impl,

)