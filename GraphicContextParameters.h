#pragma once
#ifndef GRAPHICCONTEXTPARAMETERS_H_
#define GRAPHICCONTEXTPARAMETERS_H_

#include "OpenGLAux.h"

#include <type_traits>
#include <cstdint>
#include <utility>
#include <vector>
#include <string>

template <typename T>
class GraphicContextParameters
{
	static_assert(std::is_same<float, T>::value || std::is_same<int, T>::value || std::is_same<double, T>::value || std::is_same<int64_t, T>::value, "GraphicContextParameters<T>: T must be an int, float, double or int64_t");
public:
	using ValueType = T;
public:
	std::pair<T, T> openGLVersion;
	std::pair<T, T> GLSLVersion;
	std::string vendor;
	std::string renderer;
	std::vector<std::string> extensions;

	ValueType max_compute_shader_storage_blocks;
	ValueType max_combined_shader_storage_blocks;
	ValueType max_compute_uniform_blocks;
	ValueType max_compute_texture_image_units;
	ValueType max_compute_uniform_components;
	ValueType max_compute_atomic_counters;
	ValueType max_compute_atomic_counter_buffers;
	ValueType max_combined_compute_uniform_components;
	ValueType max_compute_work_group_invocations;
	ValueType max_compute_work_group_count[3];
	ValueType max_compute_work_group_size[3];
	ValueType max_debug_group_stack_depth;
	ValueType debug_group_stack_depth;
	ValueType context_flags;
	ValueType dither;
	ValueType max_3d_texture_size;
	ValueType max_array_texture_layers;
	ValueType max_clip_distances;
	ValueType max_color_texture_samples;
	ValueType max_combined_atomic_counters;
	ValueType max_combined_fragment_uniform_components;
	ValueType max_combined_geometry_uniform_components;
	ValueType max_combined_texture_image_units;
	ValueType max_combined_uniform_blocks;
	ValueType max_combined_vertex_uniform_components;
	ValueType max_cube_map_texture_size;
	ValueType max_depth_texture_samples;
	ValueType max_draw_buffers;
	ValueType max_dual_source_draw_buffers;
	ValueType max_elements_indices;
	ValueType max_elements_vertices;
	ValueType max_fragment_atomic_counters;

	ValueType max_fragment_shader_storage_blocks;
	ValueType max_fragment_input_components;
	ValueType max_fragment_uniform_components;
	ValueType max_fragment_uniform_vectors;
	ValueType max_fragment_uniform_blocks;
	ValueType max_framebuffer_width;
	ValueType max_framebuffer_height;
	ValueType max_framebuffer_layers;
	ValueType max_framebuffer_samples;
	ValueType max_geometry_atomic_counters;
	ValueType max_geometry_shader_storage_blocks;
	ValueType max_geometry_input_components;
	ValueType max_geometry_output_components;
	ValueType max_geometry_texture_image_units;
	ValueType max_geometry_uniform_blocks;
	ValueType max_geometry_uniform_components;
	ValueType max_integer_samples;
	ValueType min_map_buffer_alignment;
	ValueType max_label_length;
	ValueType max_program_texel_offset;
	ValueType min_program_texel_offset;
	ValueType max_rectangle_texture_size;
	ValueType max_renderbuffer_size;
	ValueType max_sample_mask_words;
	ValueType max_server_wait_timeout;
	ValueType max_shader_storage_buffer_bindings;
	ValueType max_tess_control_atomic_counters;
	ValueType max_tess_evaluation_atomic_counters;
	ValueType max_tess_control_shader_storage_blocks;
	ValueType max_tess_evaluation_shader_storage_blocks;

	ValueType max_texture_buffer_size;
	ValueType max_texture_image_units;
	ValueType max_texture_lod_bias;
	ValueType max_texture_size;
	ValueType max_uniform_buffer_bindings;
	ValueType max_uniform_block_size;
	ValueType max_uniform_locations;
	ValueType max_varying_components;
	ValueType max_varying_vectors;
	ValueType max_varying_floats;
	ValueType max_vertex_atomic_counters;
	ValueType max_vertex_attribs;
	ValueType max_vertex_shader_storage_blocks;
	ValueType max_vertex_texture_image_units;
	ValueType max_vertex_uniform_components;
	ValueType max_vertex_uniform_vectors;
	ValueType max_vertex_output_components;
	ValueType max_vertex_uniform_blocks;
	ValueType max_viewport_dims[2];
	ValueType max_viewports;
	ValueType max_element_index;
	ValueType viewport_subpixel_bits;
	ValueType viewport_bounds_range[2];
private:
	void _getVersion() noexcept;
	void _getValue(GLenum token, ValueType* a) noexcept;
	void _getValue_v(GLenum token, size_t n, ValueType* a) noexcept;
	void _moveProc(GraphicContextParameters& a) noexcept;
public:
	GraphicContextParameters() noexcept;
	GraphicContextParameters(const GraphicContextParameters<T>&) = delete;
	GraphicContextParameters(GraphicContextParameters<T>&& a) noexcept;
	GraphicContextParameters<T>& operator=(const GraphicContextParameters<T>&) = delete;
	GraphicContextParameters<T>& operator=(GraphicContextParameters<T>&& a) noexcept;
	void init() noexcept;
	virtual ~GraphicContextParameters() noexcept;
};
#endif // !GRAPHICCONTEXTPARAMETERS_H_

template<>
inline void GraphicContextParameters<float>::_getValue(GLenum token, float* a) noexcept
{
	glGetFloatv(token, a);
}

//==================================================================================

template<>
inline void GraphicContextParameters<int64_t>::_getValue(GLenum token, int64_t* a) noexcept
{
	glGetInteger64v(token, a);
}

//==================================================================================

template<>
inline void GraphicContextParameters<int>::_getValue(GLenum token, int* a) noexcept
{
	glGetIntegerv(token, a);
}

//==================================================================================
#if !defined(_USE_OPENGL_ES_)
template<>
inline void GraphicContextParameters<double>::_getValue(GLenum token, double* a) noexcept
{
	glGetDoublev(token, a);
}
#endif // _USE_OPENGL_ES_
//==================================================================================

template<>
inline void GraphicContextParameters<float>::_getVersion() noexcept
{
	const GLubyte* str = glGetString(GL_VERSION);
	if (str != nullptr) {
		openGLVersion.first = (float)atoi((const char*)str);
		while (*str != 0 && *str != '.') {
			++str;
		}
		if (*str != 0 && *str == '.') {
			++str;
		}
		openGLVersion.second = (float)atoi((const char*)str);
	}
	str = glGetString(GL_SHADING_LANGUAGE_VERSION);
	if (str != nullptr) {
		GLSLVersion.first = (float)atoi((const char*)str);
		while (*str != 0 && *str != '.') {
			++str;
		}
		if (*str != 0 && *str == '.') {
			++str;
		}
		GLSLVersion.second = (float)atoi((const char*)str);
	}
}

//==================================================================================

template<>
inline void GraphicContextParameters<double>::_getVersion() noexcept
{
	const GLubyte* str = glGetString(GL_VERSION);
	if (str != nullptr) {
		openGLVersion.first = (double)atoi((const char*)str);
		while (*str != 0 && *str != '.') {
			++str;
		}
		if (*str != 0 && *str == '.') {
			++str;
		}
		openGLVersion.second = (double)atoi((const char*)str);
	}
	str = glGetString(GL_SHADING_LANGUAGE_VERSION);
	if (str != nullptr) {
		GLSLVersion.first = (double)atoi((const char*)str);
		while (*str != 0 && *str != '.') {
			++str;
		}
		if (*str != 0 && *str == '.') {
			++str;
		}
		GLSLVersion.second = (double)atoi((const char*)str);
	}
}

//==================================================================================

template<>
inline void GraphicContextParameters<int>::_getVersion() noexcept
{
	const GLubyte* str = glGetString(GL_VERSION);
	if (str != nullptr) {
		openGLVersion.first = atoi((const char*)str);
		while (*str != 0 && *str != '.') {
			++str;
		}
		if (*str != 0 && *str == '.') {
			++str;
		}
		openGLVersion.second = atoi((const char*)str);
	}
	str = glGetString(GL_SHADING_LANGUAGE_VERSION);
	if (str != nullptr) {
		GLSLVersion.first = atoi((const char*)str);
		while (*str != 0 && *str != '.') {
			++str;
		}
		if (*str != 0 && *str == '.') {
			++str;
		}
		GLSLVersion.second = atoi((const char*)str);
	}
}

//==================================================================================

template<>
inline void GraphicContextParameters<int64_t>::_getVersion() noexcept
{
	const GLubyte* str = glGetString(GL_VERSION);
	if (str != nullptr) {
		openGLVersion.first = atoll((const char*)str);
		while (*str != 0 && *str != '.') {
			++str;
		}
		if (*str != 0 && *str == '.') {
			++str;
		}
		openGLVersion.second = atoll((const char*)str);
	}
	str = glGetString(GL_SHADING_LANGUAGE_VERSION);
	if (str != nullptr) {
		GLSLVersion.first = atoll((const char*)str);
		while (*str != 0 && *str != '.') {
			++str;
		}
		if (*str != 0 && *str == '.') {
			++str;
		}
		GLSLVersion.second = atoll((const char*)str);
	}
}

//==================================================================================

template<typename T>
inline void GraphicContextParameters<T>::_getValue(GLenum token, ValueType* a) noexcept
{
}

//==================================================================================

template<>
inline void GraphicContextParameters<float>::_getValue_v(GLenum token, size_t n, float* a) noexcept
{
	for (size_t i = 0; i < n; ++i) {
		glGetFloati_v(token, (GLuint)i, a + i);
	}
}

//==================================================================================

template<>
inline void GraphicContextParameters<int>::_getValue_v(GLenum token, size_t n, int* a) noexcept
{
	for (size_t i = 0; i < n; ++i) {
		glGetIntegeri_v(token, (GLuint)i, a + i);
	}
}

//==================================================================================
#if !defined(_USE_OPENGL_ES_)
template<>
inline void GraphicContextParameters<double>::_getValue_v(GLenum token, size_t n, double* a) noexcept
{
	for (size_t i = 0; i < n; ++i) {
		glGetDoublei_v(token, (GLuint)i, a + i);
	}
}
#endif // _USE_OPENGL_ES_
//==================================================================================

template<>
inline void GraphicContextParameters<int64_t>::_getValue_v(GLenum token, size_t n, int64_t* a) noexcept
{
	for (size_t i = 0; i < n; ++i) {
		glGetInteger64i_v(token, (GLuint)i, a + i);
	}
}

//==================================================================================

template<typename T>
inline void GraphicContextParameters<T>::_getValue_v(GLenum token, size_t n, ValueType* a) noexcept
{
}

//==================================================================================

template<typename T>
inline void GraphicContextParameters<T>::_moveProc(GraphicContextParameters<T>& a) noexcept
{
	openGLVersion = a.openGLVersion;
	GLSLVersion = a.GLSLVersion;
	vendor = std::move(a.vendor);
	renderer = std::move(a.renderer);
	extensions = std::move(a.extensions);
#define _GRAPHICCONTEXTPARAMETERSMOVE_(val) val = a.val;
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_compute_shader_storage_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_combined_shader_storage_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_compute_uniform_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_compute_texture_image_units)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_compute_uniform_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_compute_atomic_counters)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_compute_atomic_counter_buffers)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_combined_compute_uniform_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_compute_work_group_invocations)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_debug_group_stack_depth)
	_GRAPHICCONTEXTPARAMETERSMOVE_(debug_group_stack_depth)
	_GRAPHICCONTEXTPARAMETERSMOVE_(context_flags)
	_GRAPHICCONTEXTPARAMETERSMOVE_(dither)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_3d_texture_size)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_array_texture_layers)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_clip_distances)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_color_texture_samples)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_combined_atomic_counters)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_combined_fragment_uniform_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_combined_geometry_uniform_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_combined_texture_image_units)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_combined_uniform_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_combined_vertex_uniform_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_cube_map_texture_size)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_depth_texture_samples)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_draw_buffers)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_dual_source_draw_buffers)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_elements_indices)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_elements_vertices)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_fragment_atomic_counters)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_fragment_shader_storage_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_fragment_input_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_fragment_uniform_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_fragment_uniform_vectors)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_fragment_uniform_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_framebuffer_width)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_framebuffer_height)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_framebuffer_layers)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_framebuffer_samples)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_geometry_atomic_counters)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_geometry_shader_storage_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_geometry_input_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_geometry_output_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_geometry_texture_image_units)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_geometry_uniform_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_geometry_uniform_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_integer_samples)
	_GRAPHICCONTEXTPARAMETERSMOVE_(min_map_buffer_alignment)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_label_length)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_label_length)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_program_texel_offset)
	_GRAPHICCONTEXTPARAMETERSMOVE_(min_program_texel_offset)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_rectangle_texture_size)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_renderbuffer_size)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_sample_mask_words)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_server_wait_timeout)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_shader_storage_buffer_bindings)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_tess_control_atomic_counters)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_tess_evaluation_atomic_counters)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_tess_control_shader_storage_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_tess_evaluation_shader_storage_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_texture_buffer_size)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_texture_image_units)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_texture_lod_bias)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_texture_size)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_uniform_buffer_bindings)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_uniform_block_size)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_uniform_locations)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_varying_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_varying_vectors)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_varying_floats)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_vertex_atomic_counters)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_vertex_attribs)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_vertex_shader_storage_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_vertex_texture_image_units)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_vertex_uniform_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_vertex_uniform_vectors)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_vertex_output_components)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_vertex_uniform_blocks)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_viewports)
	_GRAPHICCONTEXTPARAMETERSMOVE_(max_element_index)
	_GRAPHICCONTEXTPARAMETERSMOVE_(viewport_subpixel_bits)
	memcpy(max_compute_work_group_count, a.max_compute_work_group_count, sizeof(max_compute_work_group_count));
	memcpy(max_compute_work_group_size, a.max_compute_work_group_size, sizeof(max_compute_work_group_size));
	memcpy(max_viewport_dims, a.max_viewport_dims, sizeof(max_viewport_dims));
	memcpy(viewport_bounds_range, a.viewport_bounds_range, sizeof(viewport_bounds_range));

#undef _GRAPHICCONTEXTPARAMETERSMOVE_
}

//==================================================================================

template<typename T>
inline GraphicContextParameters<T>::GraphicContextParameters() noexcept :
	max_compute_shader_storage_blocks((T)0),
	max_combined_shader_storage_blocks((T)0),
	max_compute_uniform_blocks((T)0),
	max_compute_texture_image_units((T)0),
	max_compute_uniform_components((T)0),
	max_compute_atomic_counters((T)0),
	max_compute_atomic_counter_buffers((T)0),
	max_combined_compute_uniform_components((T)0),
	max_compute_work_group_invocations((T)0),
	max_compute_work_group_count{ (T)0, (T)0, (T)0 },
	max_compute_work_group_size{ (T)0, (T)0, (T)0 },
	max_debug_group_stack_depth((T)0),
	debug_group_stack_depth((T)0),
	context_flags((T)0),
	dither((T)0),
	max_3d_texture_size((T)0),
	max_array_texture_layers((T)0),
	max_clip_distances((T)0),
	max_color_texture_samples((T)0),
	max_combined_atomic_counters((T)0),
	max_combined_fragment_uniform_components((T)0),
	max_combined_geometry_uniform_components((T)0),
	max_combined_texture_image_units((T)0),
	max_combined_uniform_blocks((T)0),
	max_combined_vertex_uniform_components((T)0),
	max_cube_map_texture_size((T)0),
	max_depth_texture_samples((T)0),
	max_draw_buffers((T)0),
	max_dual_source_draw_buffers((T)0),
	max_elements_indices((T)0),
	max_elements_vertices((T)0),
	max_fragment_atomic_counters((T)0),
	max_fragment_shader_storage_blocks((T)0),
	max_fragment_input_components((T)0),
	max_fragment_uniform_components((T)0),
	max_fragment_uniform_vectors((T)0),
	max_fragment_uniform_blocks((T)0),
	max_framebuffer_width((T)0),
	max_framebuffer_height((T)0),
	max_framebuffer_layers((T)0),
	max_framebuffer_samples((T)0),
	max_geometry_atomic_counters((T)0),
	max_geometry_shader_storage_blocks((T)0),
	max_geometry_input_components((T)0),
	max_geometry_output_components((T)0),
	max_geometry_texture_image_units((T)0),
	max_geometry_uniform_blocks((T)0),
	max_geometry_uniform_components((T)0),
	max_integer_samples((T)0),
	min_map_buffer_alignment((T)0),
	max_label_length((T)0),
	max_program_texel_offset((T)0),
	min_program_texel_offset((T)0),
	max_rectangle_texture_size((T)0),
	max_renderbuffer_size((T)0),
	max_sample_mask_words((T)0),
	max_server_wait_timeout((T)0),
	max_shader_storage_buffer_bindings((T)0),
	max_tess_control_atomic_counters((T)0),
	max_tess_evaluation_atomic_counters((T)0),
	max_tess_control_shader_storage_blocks((T)0),
	max_tess_evaluation_shader_storage_blocks((T)0),
	max_texture_buffer_size((T)0),
	max_texture_image_units((T)0),
	max_texture_lod_bias((T)0),
	max_texture_size((T)0),
	max_uniform_buffer_bindings((T)0),
	max_uniform_block_size((T)0),
	max_uniform_locations((T)0),
	max_varying_components((T)0),
	max_varying_vectors((T)0),
	max_varying_floats((T)0),
	max_vertex_atomic_counters((T)0),
	max_vertex_attribs((T)0),
	max_vertex_shader_storage_blocks((T)0),
	max_vertex_texture_image_units((T)0),
	max_vertex_uniform_components((T)0),
	max_vertex_uniform_vectors((T)0),
	max_vertex_output_components((T)0),
	max_vertex_uniform_blocks((T)0),
	max_viewport_dims{ (T)0, (T)0 },
	max_viewports((T)0),
	max_element_index((T)0),
	viewport_subpixel_bits((T)0),
	viewport_bounds_range{ (T)0, (T)0 }
{
}

//==================================================================================

template<typename T>
inline GraphicContextParameters<T>::GraphicContextParameters(GraphicContextParameters<T>&& a) noexcept
{
	_moveProc(a);
}

//==================================================================================

template<typename T>
inline GraphicContextParameters<T>& GraphicContextParameters<T>::operator=(GraphicContextParameters<T>&& a) noexcept
{
	if (this != &a) {
		_moveProc(a);
	}
	return *this;
}

//==================================================================================

template<typename T>
inline void GraphicContextParameters<T>::init() noexcept
{
	const GLubyte* str = glGetString(GL_VENDOR);
	if (str != nullptr) {
		vendor = std::string((const char*)str);
	}
	str = glGetString(GL_RENDERER);
	if (str != nullptr) {
		renderer = std::string((const char*)str);
	}
	int extensionsNum = 0;
	glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsNum);
	if (extensionsNum != 0) {
		extensions.resize(extensionsNum);
		for (int i = 0; i < extensionsNum; ++i) {
			const GLubyte* extensionStr = glGetStringi(GL_EXTENSIONS, i);
			extensions[i] = std::string((const char*)extensionStr);
		}
	}
	_getVersion();
	_getValue(GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS, &max_compute_shader_storage_blocks);
	_getValue(GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, &max_combined_shader_storage_blocks);
	_getValue(GL_MAX_COMPUTE_UNIFORM_BLOCKS, &max_compute_uniform_blocks);
	_getValue(GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS, &max_compute_texture_image_units);
	_getValue(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &max_compute_uniform_components);
	_getValue(GL_MAX_COMPUTE_ATOMIC_COUNTERS, &max_compute_atomic_counters);
	_getValue(GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS, &max_combined_compute_uniform_components);
	_getValue(GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS, &max_compute_atomic_counter_buffers);
	_getValue(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &max_compute_work_group_invocations);
	_getValue_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 3, max_compute_work_group_count);
	_getValue_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 3, max_compute_work_group_size);
	_getValue(GL_MAX_DEBUG_GROUP_STACK_DEPTH, &max_debug_group_stack_depth);
	_getValue(GL_DEBUG_GROUP_STACK_DEPTH, &debug_group_stack_depth);
	_getValue(GL_CONTEXT_FLAGS, &context_flags);
	_getValue(GL_DITHER, &dither);
	_getValue(GL_MAX_3D_TEXTURE_SIZE, &max_3d_texture_size);
	_getValue(GL_MAX_ARRAY_TEXTURE_LAYERS, &max_array_texture_layers);
	_getValue(GL_MAX_CLIP_DISTANCES, &max_clip_distances);
	_getValue(GL_MAX_COLOR_TEXTURE_SAMPLES, &max_color_texture_samples);
	_getValue(GL_MAX_COMBINED_ATOMIC_COUNTERS, &max_combined_atomic_counters);
	_getValue(GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS, &max_combined_fragment_uniform_components);
	_getValue(GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS, &max_combined_geometry_uniform_components);
	_getValue(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max_combined_texture_image_units);
	_getValue(GL_MAX_COMBINED_UNIFORM_BLOCKS, &max_combined_uniform_blocks);
	_getValue(GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS, &max_combined_vertex_uniform_components);
	_getValue(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &max_cube_map_texture_size);
	_getValue(GL_MAX_DEPTH_TEXTURE_SAMPLES, &max_depth_texture_samples);
	_getValue(GL_MAX_DRAW_BUFFERS, &max_draw_buffers);
	_getValue(GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, &max_dual_source_draw_buffers);
	_getValue(GL_MAX_ELEMENTS_INDICES, &max_elements_indices);
	_getValue(GL_MAX_ELEMENTS_VERTICES, &max_elements_vertices);
	_getValue(GL_MAX_FRAGMENT_ATOMIC_COUNTERS, &max_fragment_atomic_counters);
	_getValue(GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS, &max_fragment_shader_storage_blocks);
	_getValue(GL_MAX_FRAGMENT_INPUT_COMPONENTS, &max_fragment_input_components);
	_getValue(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &max_fragment_uniform_components);
	_getValue(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &max_fragment_uniform_vectors);
	_getValue(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &max_fragment_uniform_blocks);
	_getValue(GL_MAX_FRAMEBUFFER_WIDTH, &max_framebuffer_width);
	_getValue(GL_MAX_FRAMEBUFFER_HEIGHT, &max_framebuffer_height);
	_getValue(GL_MAX_FRAMEBUFFER_LAYERS, &max_framebuffer_layers);
	_getValue(GL_MAX_FRAMEBUFFER_SAMPLES, &max_framebuffer_samples);
	_getValue(GL_MAX_GEOMETRY_ATOMIC_COUNTERS, &max_geometry_atomic_counters);
	_getValue(GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS, &max_geometry_shader_storage_blocks);
	_getValue(GL_MAX_GEOMETRY_INPUT_COMPONENTS, &max_geometry_input_components);
	_getValue(GL_MAX_GEOMETRY_OUTPUT_COMPONENTS, &max_geometry_output_components);
	_getValue(GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS, &max_geometry_texture_image_units);
	_getValue(GL_MAX_GEOMETRY_UNIFORM_BLOCKS, &max_geometry_uniform_blocks);
	_getValue(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &max_geometry_uniform_components);
	_getValue(GL_MAX_INTEGER_SAMPLES, &max_integer_samples);
	_getValue(GL_MIN_MAP_BUFFER_ALIGNMENT, &min_map_buffer_alignment);
	_getValue(GL_MAX_LABEL_LENGTH, &max_label_length);
	_getValue(GL_MAX_PROGRAM_TEXEL_OFFSET, &max_program_texel_offset);
	_getValue(GL_MIN_PROGRAM_TEXEL_OFFSET, &min_program_texel_offset);
	_getValue(GL_MAX_RECTANGLE_TEXTURE_SIZE, &max_rectangle_texture_size);
	_getValue(GL_MAX_RENDERBUFFER_SIZE, &max_renderbuffer_size);
	_getValue(GL_MAX_SAMPLE_MASK_WORDS, &max_sample_mask_words);
	_getValue(GL_MAX_SERVER_WAIT_TIMEOUT, &max_server_wait_timeout);
	_getValue(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &max_shader_storage_buffer_bindings);
	_getValue(GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS, &max_tess_control_atomic_counters);
	_getValue(GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS, &max_tess_evaluation_atomic_counters);
	_getValue(GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS, &max_tess_control_shader_storage_blocks);
	_getValue(GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS, &max_tess_evaluation_shader_storage_blocks);

	_getValue(GL_MAX_TEXTURE_BUFFER_SIZE, &max_texture_buffer_size);
	_getValue(GL_MAX_TEXTURE_IMAGE_UNITS, &max_texture_image_units);
	_getValue(GL_MAX_TEXTURE_LOD_BIAS, &max_texture_lod_bias);
	_getValue(GL_MAX_TEXTURE_SIZE, &max_texture_size);
	_getValue(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max_uniform_buffer_bindings);
	_getValue(GL_MAX_UNIFORM_BLOCK_SIZE, &max_uniform_block_size);
	_getValue(GL_MAX_UNIFORM_LOCATIONS, &max_uniform_locations);
	_getValue(GL_MAX_VARYING_COMPONENTS, &max_varying_components);
	_getValue(GL_MAX_VARYING_VECTORS, &max_varying_vectors);
	_getValue(GL_MAX_VARYING_FLOATS, &max_varying_floats);
	_getValue(GL_MAX_VERTEX_ATOMIC_COUNTERS, &max_vertex_atomic_counters);
	_getValue(GL_MAX_VERTEX_ATTRIBS, &max_vertex_attribs);
	_getValue(GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS, &max_vertex_shader_storage_blocks);
	_getValue(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &max_vertex_texture_image_units);
	_getValue(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &max_vertex_uniform_components);
	_getValue(GL_MAX_VERTEX_UNIFORM_VECTORS, &max_vertex_uniform_vectors);
	_getValue(GL_MAX_VERTEX_OUTPUT_COMPONENTS, &max_vertex_output_components);
	_getValue(GL_MAX_VERTEX_UNIFORM_BLOCKS, &max_vertex_uniform_blocks);
	_getValue_v(GL_MAX_VIEWPORT_DIMS, 2, max_viewport_dims);
	_getValue(GL_MAX_VIEWPORTS, &max_viewports);
	_getValue(GL_MAX_ELEMENT_INDEX, &max_element_index);
	_getValue(GL_VIEWPORT_SUBPIXEL_BITS, &viewport_subpixel_bits);
	_getValue_v(GL_VIEWPORT_BOUNDS_RANGE, 2, viewport_bounds_range);
}

//==================================================================================

template<typename T>
inline GraphicContextParameters<T>::~GraphicContextParameters() noexcept
{
}
