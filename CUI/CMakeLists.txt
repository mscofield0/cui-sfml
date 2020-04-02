# -----------------------------

cmake_minimum_required(VERSION 3.14)
project(CUI)


# -----------------------------

set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

set(HEADER_FILES ${INCLUDE_DIR}/aliases.hpp
    ${INCLUDE_DIR}/compile_time/data_types/color.hpp
    ${INCLUDE_DIR}/compile_time/data_types/data_types.hpp
    ${INCLUDE_DIR}/compile_time/data_types/instruction.hpp
    ${INCLUDE_DIR}/compile_time/data_types/vec.hpp
    ${INCLUDE_DIR}/compile_time/format/detail/consume.hpp
    ${INCLUDE_DIR}/compile_time/format/detail/log10.hpp
    ${INCLUDE_DIR}/compile_time/format/detail/pow.hpp
    ${INCLUDE_DIR}/compile_time/format/detail/size_information.hpp
    ${INCLUDE_DIR}/compile_time/format/fmt.hpp
    ${INCLUDE_DIR}/compile_time/format/fmt_string_view.hpp
    ${INCLUDE_DIR}/compile_time/format/format.hpp
    ${INCLUDE_DIR}/compile_time/mem_utils/memcpy.hpp
    ${INCLUDE_DIR}/compile_time/mem_utils/memmove.hpp
    ${INCLUDE_DIR}/compile_time/mem_utils/memset.hpp
    ${INCLUDE_DIR}/compile_time/scene.hpp
    ${INCLUDE_DIR}/compile_time/scenes/block.hpp
    ${INCLUDE_DIR}/compile_time/scenes/detail/count_amount_of_blocks.hpp
    ${INCLUDE_DIR}/compile_time/scenes/detail/count_max_depth.hpp
    ${INCLUDE_DIR}/compile_time/scenes/detail/count_tabs.hpp
    ${INCLUDE_DIR}/compile_time/scenes/detail/find_parent.hpp
    ${INCLUDE_DIR}/compile_time/scenes/detail/valid_block_name.hpp
    ${INCLUDE_DIR}/compile_time/scenes/parse_scenes.hpp
    ${INCLUDE_DIR}/compile_time/stream/stream.hpp
    ${INCLUDE_DIR}/compile_time/stream/stream_utils.hpp
    ${INCLUDE_DIR}/compile_time/string/char_traits.hpp
    ${INCLUDE_DIR}/compile_time/string/string_view.hpp
    ${INCLUDE_DIR}/compile_time/style.hpp
    ${INCLUDE_DIR}/compile_time/styles/attribute.hpp
    ${INCLUDE_DIR}/compile_time/styles/attribute_data.hpp
    ${INCLUDE_DIR}/compile_time/styles/data_with_rule.hpp
    ${INCLUDE_DIR}/compile_time/styles/definition.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/constants.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/helper_macros.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/styles/count_definitions.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/styles/get_attribute_index.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/valid_attribute_types.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/char_checks.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/dt_to_vat.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/function_argument_list.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/functions/dependencies.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/functions/is_in_set.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/functions/rgb.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/functions/rgba.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/functions/validate_arg_types.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/functions/vec.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/index_of_function_name.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/is_correct_value_for_attr_type.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/lookup_function.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/lookup_string.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/parse_float.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/parse_int.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/parse_percentage.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/pow.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/validate_attribute_type.hpp
    ${INCLUDE_DIR}/compile_time/styles/detail/values/value_type.hpp
    ${INCLUDE_DIR}/compile_time/styles/parse_attribute_value.hpp
    ${INCLUDE_DIR}/compile_time/styles/parse_styles.hpp
    ${INCLUDE_DIR}/compile_time/value_data.hpp
    ${INCLUDE_DIR}/compile_time/variant/variant.hpp
    ${INCLUDE_DIR}/containers/detail/nary_tree/const_node_iterator.hpp
    ${INCLUDE_DIR}/containers/detail/nary_tree/node_iterator.hpp
    ${INCLUDE_DIR}/containers/nary_tree.hpp
    ${INCLUDE_DIR}/containers/static_stack.hpp
    ${INCLUDE_DIR}/containers/static_vector.hpp
    ${INCLUDE_DIR}/containers/tracked_vector.hpp
    ${INCLUDE_DIR}/containers/vector.hpp
    ${INCLUDE_DIR}/engine/base_event_manager.hpp
    ${INCLUDE_DIR}/engine/detail/has/cache.hpp
    ${INCLUDE_DIR}/engine/detail/has/poll_event.hpp
    ${INCLUDE_DIR}/mem_profiler/memcounter.hpp
    ${INCLUDE_DIR}/mem_profiler/memtrack.hpp
    ${INCLUDE_DIR}/utils/abs.hpp
    ${INCLUDE_DIR}/utils/declval.hpp
    ${INCLUDE_DIR}/utils/detail/declval_impl.hpp
    ${INCLUDE_DIR}/utils/detail/integral_constant.hpp
    ${INCLUDE_DIR}/utils/detail/is_lvalue_reference.hpp
    ${INCLUDE_DIR}/utils/detail/remove_reference.hpp
    ${INCLUDE_DIR}/utils/distance.hpp
    ${INCLUDE_DIR}/utils/forward.hpp
    ${INCLUDE_DIR}/utils/make_array.hpp
    ${INCLUDE_DIR}/utils/min.hpp
    ${INCLUDE_DIR}/utils/move.hpp
    ${INCLUDE_DIR}/utils/void_t.hpp
    ${INCLUDE_DIR}/value_data.hpp
    ${INCLUDE_DIR}/visual/attributes.hpp
    ${INCLUDE_DIR}/visual/base_render_context.hpp
    ${INCLUDE_DIR}/visual/node.hpp
    ${INCLUDE_DIR}/visual/rules.hpp
    ${INCLUDE_DIR}/visual/scene_graph.hpp
    ${INCLUDE_DIR}/visual/schematic.hpp
    ${INCLUDE_DIR}/window.hpp
)

# -----------------------------

add_library(CUI INTERFACE)

target_sources(CUI INTERFACE "$<BUILD_INTERFACE:${HEADER_FILES}>")
target_include_directories(CUI INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>)
target_include_directories(CUI SYSTEM INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)


# install(TARGETS CUI 
#     EXPORT CUIConfig.cmake
#     RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT bin
#     LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT bin
#     ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT devel
#     FRAMEWORK DESTINATION "." COMPONENT bin)
# 
# export(TARGETS CUI
#     NAMESPACE CUI::
#     FILE "${CMAKE_CURRENT_BINARY_DIR}/CUIConfig.cmake"
# )
# install(EXPORT CUIConfig.cmake
#     DESTINATION "${CMAKE_INSTALL_DATADIR}/CUI/cmake"
#     NAMESPACE CUI::
# )