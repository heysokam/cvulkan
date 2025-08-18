//:___________________________________________________________
//  cvulkan  |  Copyright (C) Ivan Mar (sOkam!)  |  MPL-2.0  :
//:___________________________________________________________
#include "../device.h"


cvk_Pure cvk_device_Features cvk_device_features_empty () {
  cvk_device_Features result = (cvk_device_Features){ 0 };
#ifdef VK_VERSION_1_1
  result.list.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
  result.v1_1.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
  result.list.pNext = &result.v1_1;
#endif  // VK_VERSION_1_1
#ifdef VK_VERSION_1_2
  result.v1_1.pNext = &result.v1_2;
  result.v1_2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
#endif  // VK_VERSION_1_2
#ifdef VK_VERSION_1_3
  result.v1_2.pNext = &result.v1_3;
  result.v1_3.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
#endif  // VK_VERSION_1_3
#ifdef VK_VERSION_1_4
  result.v1_3.pNext = &result.v1_4;
  result.v1_4.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES;
#endif  // VK_VERSION_1_4
  return result;
}  //:: cvk_device_features_empty


cvk_Pure cvk_device_features_Required cvk_device_features_required_defaults () {
  cvk_device_features_Required result = (cvk_device_features_Required) /* clang-format off */ {
    .user    = cvk_device_features_empty(),
    .cvulkan = &(cvk_device_Features){ 0 },
  };  // clang-format on
  *result.cvulkan = cvk_device_features_empty();
  //________________________________________________
  // Features required for cvulkan default behavior
  //  : v1.0
  VkPhysicalDeviceFeatures v1_0 = (VkPhysicalDeviceFeatures){
    .samplerAnisotropy = VK_TRUE,
  };
  //________________________________________________
  // Apply to the result
#ifndef VK_VERSION_1_1
  result.cvulkan->list = v1_0;
#else   // >= VK_VERSION_1_1
  result.cvulkan->v1_0          = v1_0;
  result.cvulkan->list.features = result.cvulkan->v1_0;
#endif  // >= VK_VERSION_1_1
  return result;
}


cvk_Pure cvk_device_Features cvk_device_features_getSupported (
  cvk_device_Physical const* const device
) {
  cvk_device_Features result = cvk_device_features_empty();

  //__________________________________________________
  // Create the versions chain and request them
#ifndef VK_VERSION_1_1
  vkGetPhysicalDeviceFeatures(device->ct, &result.list);
#else   // >= VK_VERSION_1_1
  result.list.pNext = &result.v1_1;
#ifdef VK_VERSION_1_2
  result.v1_1.pNext = &result.v1_2;
#endif  // VK_VERSION_1_2
#ifdef VK_VERSION_1_3
  result.v1_2.pNext = &result.v1_3;
#endif  // VK_VERSION_1_3
#ifdef VK_VERSION_1_4
  result.v1_3.pNext = &result.v1_4;
#endif  // VK_VERSION_1_4
  vkGetPhysicalDeviceFeatures2(device->ct, &result.list);
  result.list.features = result.v1_0;
#endif  // >= VK_VERSION_1_1

  //__________________________________________________
  // Return the result
  return result;
}


cvk_Pure cvk_bool cvk_device_features_supported (
  cvk_device_Physical const* const device,
  cvk_device_Features const* const required
) {
  if (!required) return cvk_true;  // Exit early if we are checking no features
  cvk_device_Features const supported = cvk_device_features_getSupported(device);
  //________________________________________________
  // Check v1.0
#ifndef VK_VERSION_1_1
  VkPhysicalDeviceFeatures const supported_v1_0 = supported.list;
  VkPhysicalDeviceFeatures const required_v1_0  = required.list;
#else   // >= VK_VERSION_1_1
  VkPhysicalDeviceFeatures const supported_v1_0 = supported.v1_0;
  VkPhysicalDeviceFeatures const required_v1_0  = required->v1_0;
#endif  // >= VK_VERSION_1_1
  if (!supported_v1_0.robustBufferAccess && required_v1_0.robustBufferAccess) return cvk_false;
  if (!supported_v1_0.fullDrawIndexUint32 && required_v1_0.fullDrawIndexUint32) return cvk_false;
  if (!supported_v1_0.imageCubeArray && required_v1_0.imageCubeArray) return cvk_false;
  if (!supported_v1_0.independentBlend && required_v1_0.independentBlend) return cvk_false;
  if (!supported_v1_0.geometryShader && required_v1_0.geometryShader) return cvk_false;
  if (!supported_v1_0.tessellationShader && required_v1_0.tessellationShader) return cvk_false;
  if (!supported_v1_0.sampleRateShading && required_v1_0.sampleRateShading) return cvk_false;
  if (!supported_v1_0.dualSrcBlend && required_v1_0.dualSrcBlend) return cvk_false;
  if (!supported_v1_0.logicOp && required_v1_0.logicOp) return cvk_false;
  if (!supported_v1_0.multiDrawIndirect && required_v1_0.multiDrawIndirect) return cvk_false;
  if (!supported_v1_0.drawIndirectFirstInstance && required_v1_0.drawIndirectFirstInstance) return cvk_false;
  if (!supported_v1_0.depthClamp && required_v1_0.depthClamp) return cvk_false;
  if (!supported_v1_0.depthBiasClamp && required_v1_0.depthBiasClamp) return cvk_false;
  if (!supported_v1_0.fillModeNonSolid && required_v1_0.fillModeNonSolid) return cvk_false;
  if (!supported_v1_0.depthBounds && required_v1_0.depthBounds) return cvk_false;
  if (!supported_v1_0.wideLines && required_v1_0.wideLines) return cvk_false;
  if (!supported_v1_0.largePoints && required_v1_0.largePoints) return cvk_false;
  if (!supported_v1_0.alphaToOne && required_v1_0.alphaToOne) return cvk_false;
  if (!supported_v1_0.multiViewport && required_v1_0.multiViewport) return cvk_false;
  if (!supported_v1_0.samplerAnisotropy && required_v1_0.samplerAnisotropy) return cvk_false;
  if (!supported_v1_0.textureCompressionETC2 && required_v1_0.textureCompressionETC2) return cvk_false;
  if (!supported_v1_0.textureCompressionASTC_LDR && required_v1_0.textureCompressionASTC_LDR) return cvk_false;
  if (!supported_v1_0.textureCompressionBC && required_v1_0.textureCompressionBC) return cvk_false;
  if (!supported_v1_0.occlusionQueryPrecise && required_v1_0.occlusionQueryPrecise) return cvk_false;
  if (!supported_v1_0.pipelineStatisticsQuery && required_v1_0.pipelineStatisticsQuery) return cvk_false;
  if (!supported_v1_0.vertexPipelineStoresAndAtomics && required_v1_0.vertexPipelineStoresAndAtomics) return cvk_false;
  if (!supported_v1_0.fragmentStoresAndAtomics && required_v1_0.fragmentStoresAndAtomics) return cvk_false;
  if (!supported_v1_0.shaderTessellationAndGeometryPointSize && required_v1_0.shaderTessellationAndGeometryPointSize) return cvk_false;
  if (!supported_v1_0.shaderImageGatherExtended && required_v1_0.shaderImageGatherExtended) return cvk_false;
  if (!supported_v1_0.shaderStorageImageExtendedFormats && required_v1_0.shaderStorageImageExtendedFormats) return cvk_false;
  if (!supported_v1_0.shaderStorageImageMultisample && required_v1_0.shaderStorageImageMultisample) return cvk_false;
  if (!supported_v1_0.shaderStorageImageReadWithoutFormat && required_v1_0.shaderStorageImageReadWithoutFormat) return cvk_false;
  if (!supported_v1_0.shaderStorageImageWriteWithoutFormat && required_v1_0.shaderStorageImageWriteWithoutFormat) return cvk_false;
  if (!supported_v1_0.shaderUniformBufferArrayDynamicIndexing && required_v1_0.shaderUniformBufferArrayDynamicIndexing) return cvk_false;
  if (!supported_v1_0.shaderSampledImageArrayDynamicIndexing && required_v1_0.shaderSampledImageArrayDynamicIndexing) return cvk_false;
  if (!supported_v1_0.shaderStorageBufferArrayDynamicIndexing && required_v1_0.shaderStorageBufferArrayDynamicIndexing) return cvk_false;
  if (!supported_v1_0.shaderStorageImageArrayDynamicIndexing && required_v1_0.shaderStorageImageArrayDynamicIndexing) return cvk_false;
  if (!supported_v1_0.shaderClipDistance && required_v1_0.shaderClipDistance) return cvk_false;
  if (!supported_v1_0.shaderCullDistance && required_v1_0.shaderCullDistance) return cvk_false;
  if (!supported_v1_0.shaderFloat64 && required_v1_0.shaderFloat64) return cvk_false;
  if (!supported_v1_0.shaderInt64 && required_v1_0.shaderInt64) return cvk_false;
  if (!supported_v1_0.shaderInt16 && required_v1_0.shaderInt16) return cvk_false;
  if (!supported_v1_0.shaderResourceResidency && required_v1_0.shaderResourceResidency) return cvk_false;
  if (!supported_v1_0.shaderResourceMinLod && required_v1_0.shaderResourceMinLod) return cvk_false;
  if (!supported_v1_0.sparseBinding && required_v1_0.sparseBinding) return cvk_false;
  if (!supported_v1_0.sparseResidencyBuffer && required_v1_0.sparseResidencyBuffer) return cvk_false;
  if (!supported_v1_0.sparseResidencyImage2D && required_v1_0.sparseResidencyImage2D) return cvk_false;
  if (!supported_v1_0.sparseResidencyImage3D && required_v1_0.sparseResidencyImage3D) return cvk_false;
  if (!supported_v1_0.sparseResidency2Samples && required_v1_0.sparseResidency2Samples) return cvk_false;
  if (!supported_v1_0.sparseResidency4Samples && required_v1_0.sparseResidency4Samples) return cvk_false;
  if (!supported_v1_0.sparseResidency8Samples && required_v1_0.sparseResidency8Samples) return cvk_false;
  if (!supported_v1_0.sparseResidency16Samples && required_v1_0.sparseResidency16Samples) return cvk_false;
  if (!supported_v1_0.sparseResidencyAliased && required_v1_0.sparseResidencyAliased) return cvk_false;
  if (!supported_v1_0.variableMultisampleRate && required_v1_0.variableMultisampleRate) return cvk_false;
  if (!supported_v1_0.inheritedQueries && required_v1_0.inheritedQueries) return cvk_false;

  //________________________________________________
  // Check v1.1
#ifdef VK_VERSION_1_1
  if (!supported.v1_1.storageBuffer16BitAccess && required->v1_1.storageBuffer16BitAccess) return cvk_false;
  if (!supported.v1_1.uniformAndStorageBuffer16BitAccess && required->v1_1.uniformAndStorageBuffer16BitAccess) return cvk_false;
  if (!supported.v1_1.storagePushConstant16 && required->v1_1.storagePushConstant16) return cvk_false;
  if (!supported.v1_1.storageInputOutput16 && required->v1_1.storageInputOutput16) return cvk_false;
  if (!supported.v1_1.multiview && required->v1_1.multiview) return cvk_false;
  if (!supported.v1_1.multiviewGeometryShader && required->v1_1.multiviewGeometryShader) return cvk_false;
  if (!supported.v1_1.multiviewTessellationShader && required->v1_1.multiviewTessellationShader) return cvk_false;
  if (!supported.v1_1.variablePointersStorageBuffer && required->v1_1.variablePointersStorageBuffer) return cvk_false;
  if (!supported.v1_1.variablePointers && required->v1_1.variablePointers) return cvk_false;
  if (!supported.v1_1.protectedMemory && required->v1_1.protectedMemory) return cvk_false;
  if (!supported.v1_1.samplerYcbcrConversion && required->v1_1.samplerYcbcrConversion) return cvk_false;
  if (!supported.v1_1.shaderDrawParameters && required->v1_1.shaderDrawParameters) return cvk_false;
#endif  // >= VK_VERSION_1_1

  //________________________________________________
  // Check v1.2
#ifdef VK_VERSION_1_2
  if (!supported.v1_2.samplerMirrorClampToEdge && required->v1_2.samplerMirrorClampToEdge) return cvk_false;
  if (!supported.v1_2.drawIndirectCount && required->v1_2.drawIndirectCount) return cvk_false;
  if (!supported.v1_2.storageBuffer8BitAccess && required->v1_2.storageBuffer8BitAccess) return cvk_false;
  if (!supported.v1_2.uniformAndStorageBuffer8BitAccess && required->v1_2.uniformAndStorageBuffer8BitAccess) return cvk_false;
  if (!supported.v1_2.storagePushConstant8 && required->v1_2.storagePushConstant8) return cvk_false;
  if (!supported.v1_2.shaderBufferInt64Atomics && required->v1_2.shaderBufferInt64Atomics) return cvk_false;
  if (!supported.v1_2.shaderSharedInt64Atomics && required->v1_2.shaderSharedInt64Atomics) return cvk_false;
  if (!supported.v1_2.shaderFloat16 && required->v1_2.shaderFloat16) return cvk_false;
  if (!supported.v1_2.shaderInt8 && required->v1_2.shaderInt8) return cvk_false;
  if (!supported.v1_2.descriptorIndexing && required->v1_2.descriptorIndexing) return cvk_false;
  if (!supported.v1_2.shaderInputAttachmentArrayDynamicIndexing && required->v1_2.shaderInputAttachmentArrayDynamicIndexing) return cvk_false;
  if (!supported.v1_2.shaderUniformTexelBufferArrayDynamicIndexing && required->v1_2.shaderUniformTexelBufferArrayDynamicIndexing) return cvk_false;
  if (!supported.v1_2.shaderStorageTexelBufferArrayDynamicIndexing && required->v1_2.shaderStorageTexelBufferArrayDynamicIndexing) return cvk_false;
  if (!supported.v1_2.shaderUniformBufferArrayNonUniformIndexing && required->v1_2.shaderUniformBufferArrayNonUniformIndexing) return cvk_false;
  if (!supported.v1_2.shaderSampledImageArrayNonUniformIndexing && required->v1_2.shaderSampledImageArrayNonUniformIndexing) return cvk_false;
  if (!supported.v1_2.shaderStorageBufferArrayNonUniformIndexing && required->v1_2.shaderStorageBufferArrayNonUniformIndexing) return cvk_false;
  if (!supported.v1_2.shaderStorageImageArrayNonUniformIndexing && required->v1_2.shaderStorageImageArrayNonUniformIndexing) return cvk_false;
  if (!supported.v1_2.shaderInputAttachmentArrayNonUniformIndexing && required->v1_2.shaderInputAttachmentArrayNonUniformIndexing) return cvk_false;
  if (!supported.v1_2.shaderUniformTexelBufferArrayNonUniformIndexing && required->v1_2.shaderUniformTexelBufferArrayNonUniformIndexing) return cvk_false;
  if (!supported.v1_2.shaderStorageTexelBufferArrayNonUniformIndexing && required->v1_2.shaderStorageTexelBufferArrayNonUniformIndexing) return cvk_false;
  if (!supported.v1_2.descriptorBindingUniformBufferUpdateAfterBind && required->v1_2.descriptorBindingUniformBufferUpdateAfterBind) return cvk_false;
  if (!supported.v1_2.descriptorBindingSampledImageUpdateAfterBind && required->v1_2.descriptorBindingSampledImageUpdateAfterBind) return cvk_false;
  if (!supported.v1_2.descriptorBindingStorageImageUpdateAfterBind && required->v1_2.descriptorBindingStorageImageUpdateAfterBind) return cvk_false;
  if (!supported.v1_2.descriptorBindingStorageBufferUpdateAfterBind && required->v1_2.descriptorBindingStorageBufferUpdateAfterBind) return cvk_false;
  if (!supported.v1_2.descriptorBindingUniformTexelBufferUpdateAfterBind && required->v1_2.descriptorBindingUniformTexelBufferUpdateAfterBind) return cvk_false;
  if (!supported.v1_2.descriptorBindingStorageTexelBufferUpdateAfterBind && required->v1_2.descriptorBindingStorageTexelBufferUpdateAfterBind) return cvk_false;
  if (!supported.v1_2.descriptorBindingUpdateUnusedWhilePending && required->v1_2.descriptorBindingUpdateUnusedWhilePending) return cvk_false;
  if (!supported.v1_2.descriptorBindingPartiallyBound && required->v1_2.descriptorBindingPartiallyBound) return cvk_false;
  if (!supported.v1_2.descriptorBindingVariableDescriptorCount && required->v1_2.descriptorBindingVariableDescriptorCount) return cvk_false;
  if (!supported.v1_2.runtimeDescriptorArray && required->v1_2.runtimeDescriptorArray) return cvk_false;
  if (!supported.v1_2.samplerFilterMinmax && required->v1_2.samplerFilterMinmax) return cvk_false;
  if (!supported.v1_2.scalarBlockLayout && required->v1_2.scalarBlockLayout) return cvk_false;
  if (!supported.v1_2.imagelessFramebuffer && required->v1_2.imagelessFramebuffer) return cvk_false;
  if (!supported.v1_2.uniformBufferStandardLayout && required->v1_2.uniformBufferStandardLayout) return cvk_false;
  if (!supported.v1_2.shaderSubgroupExtendedTypes && required->v1_2.shaderSubgroupExtendedTypes) return cvk_false;
  if (!supported.v1_2.separateDepthStencilLayouts && required->v1_2.separateDepthStencilLayouts) return cvk_false;
  if (!supported.v1_2.hostQueryReset && required->v1_2.hostQueryReset) return cvk_false;
  if (!supported.v1_2.timelineSemaphore && required->v1_2.timelineSemaphore) return cvk_false;
  if (!supported.v1_2.bufferDeviceAddress && required->v1_2.bufferDeviceAddress) return cvk_false;
  if (!supported.v1_2.bufferDeviceAddressCaptureReplay && required->v1_2.bufferDeviceAddressCaptureReplay) return cvk_false;
  if (!supported.v1_2.bufferDeviceAddressMultiDevice && required->v1_2.bufferDeviceAddressMultiDevice) return cvk_false;
  if (!supported.v1_2.vulkanMemoryModel && required->v1_2.vulkanMemoryModel) return cvk_false;
  if (!supported.v1_2.vulkanMemoryModelDeviceScope && required->v1_2.vulkanMemoryModelDeviceScope) return cvk_false;
  if (!supported.v1_2.vulkanMemoryModelAvailabilityVisibilityChains && required->v1_2.vulkanMemoryModelAvailabilityVisibilityChains) return cvk_false;
  if (!supported.v1_2.shaderOutputViewportIndex && required->v1_2.shaderOutputViewportIndex) return cvk_false;
  if (!supported.v1_2.shaderOutputLayer && required->v1_2.shaderOutputLayer) return cvk_false;
  if (!supported.v1_2.subgroupBroadcastDynamicId && required->v1_2.subgroupBroadcastDynamicId) return cvk_false;
#endif  // >= VK_VERSION_1_2

  //________________________________________________
  // Check v1.3
#ifdef VK_VERSION_1_3
  if (!supported.v1_3.robustImageAccess && required->v1_3.robustImageAccess) return cvk_false;
  if (!supported.v1_3.inlineUniformBlock && required->v1_3.inlineUniformBlock) return cvk_false;
  if (!supported.v1_3.descriptorBindingInlineUniformBlockUpdateAfterBind && required->v1_3.descriptorBindingInlineUniformBlockUpdateAfterBind) return cvk_false;
  if (!supported.v1_3.pipelineCreationCacheControl && required->v1_3.pipelineCreationCacheControl) return cvk_false;
  if (!supported.v1_3.privateData && required->v1_3.privateData) return cvk_false;
  if (!supported.v1_3.shaderDemoteToHelperInvocation && required->v1_3.shaderDemoteToHelperInvocation) return cvk_false;
  if (!supported.v1_3.shaderTerminateInvocation && required->v1_3.shaderTerminateInvocation) return cvk_false;
  if (!supported.v1_3.subgroupSizeControl && required->v1_3.subgroupSizeControl) return cvk_false;
  if (!supported.v1_3.computeFullSubgroups && required->v1_3.computeFullSubgroups) return cvk_false;
  if (!supported.v1_3.synchronization2 && required->v1_3.synchronization2) return cvk_false;
  if (!supported.v1_3.textureCompressionASTC_HDR && required->v1_3.textureCompressionASTC_HDR) return cvk_false;
  if (!supported.v1_3.shaderZeroInitializeWorkgroupMemory && required->v1_3.shaderZeroInitializeWorkgroupMemory) return cvk_false;
  if (!supported.v1_3.dynamicRendering && required->v1_3.dynamicRendering) return cvk_false;
  if (!supported.v1_3.shaderIntegerDotProduct && required->v1_3.shaderIntegerDotProduct) return cvk_false;
  if (!supported.v1_3.maintenance4 && required->v1_3.maintenance4) return cvk_false;
#endif  // >= VK_VERSION_1_3

  //________________________________________________
  // Check v1.4
#ifdef VK_VERSION_1_4
  if (!supported.v1_4.globalPriorityQuery && required->v1_4.globalPriorityQuery) return cvk_false;
  if (!supported.v1_4.shaderSubgroupRotate && required->v1_4.shaderSubgroupRotate) return cvk_false;
  if (!supported.v1_4.shaderSubgroupRotateClustered && required->v1_4.shaderSubgroupRotateClustered) return cvk_false;
  if (!supported.v1_4.shaderFloatControls2 && required->v1_4.shaderFloatControls2) return cvk_false;
  if (!supported.v1_4.shaderExpectAssume && required->v1_4.shaderExpectAssume) return cvk_false;
  if (!supported.v1_4.rectangularLines && required->v1_4.rectangularLines) return cvk_false;
  if (!supported.v1_4.bresenhamLines && required->v1_4.bresenhamLines) return cvk_false;
  if (!supported.v1_4.smoothLines && required->v1_4.smoothLines) return cvk_false;
  if (!supported.v1_4.stippledRectangularLines && required->v1_4.stippledRectangularLines) return cvk_false;
  if (!supported.v1_4.stippledBresenhamLines && required->v1_4.stippledBresenhamLines) return cvk_false;
  if (!supported.v1_4.stippledSmoothLines && required->v1_4.stippledSmoothLines) return cvk_false;
  if (!supported.v1_4.vertexAttributeInstanceRateDivisor && required->v1_4.vertexAttributeInstanceRateDivisor) return cvk_false;
  if (!supported.v1_4.vertexAttributeInstanceRateZeroDivisor && required->v1_4.vertexAttributeInstanceRateZeroDivisor) return cvk_false;
  if (!supported.v1_4.indexTypeUint8 && required->v1_4.indexTypeUint8) return cvk_false;
  if (!supported.v1_4.dynamicRenderingLocalRead && required->v1_4.dynamicRenderingLocalRead) return cvk_false;
  if (!supported.v1_4.maintenance5 && required->v1_4.maintenance5) return cvk_false;
  if (!supported.v1_4.maintenance6 && required->v1_4.maintenance6) return cvk_false;
  if (!supported.v1_4.pipelineProtectedAccess && required->v1_4.pipelineProtectedAccess) return cvk_false;
  if (!supported.v1_4.pipelineRobustness && required->v1_4.pipelineRobustness) return cvk_false;
  if (!supported.v1_4.hostImageCopy && required->v1_4.hostImageCopy) return cvk_false;
  if (!supported.v1_4.pushDescriptor && required->v1_4.pushDescriptor) return cvk_false;
#endif  // >= VK_VERSION_1_4

  //________________________________________________
  // All features are supported, not required or omitted.
  return cvk_true;
}  //:: cvk_device_features_supported


cvk_Pure cvk_device_Features cvk_device_features_create (
  cvk_device_features_Required const* const features
) {
  cvk_device_Features result = cvk_device_features_empty();
  // Find the list of required extensions
  cvk_device_features_Required required_defaults = cvk_device_features_required_defaults();
  cvk_device_features_Required required          = (features) ? *features : required_defaults;
  if (!required.cvulkan) required.cvulkan = required_defaults.cvulkan;

  //________________________________________________
  // Merge v1.0
#ifndef VK_VERSION_1_1
  VkPhysicalDeviceFeatures const user_v1_0    = required.user.list;
  VkPhysicalDeviceFeatures const cvulkan_v1_0 = required.cvulkan->list;
  VkPhysicalDeviceFeatures*      result_v1_0  = &result.cvulkan->list;
#else   // >= VK_VERSION_1_1
  VkPhysicalDeviceFeatures const user_v1_0    = required.user.v1_0;
  VkPhysicalDeviceFeatures const cvulkan_v1_0 = required.cvulkan->v1_0;
  VkPhysicalDeviceFeatures*      result_v1_0  = &result.v1_0;
#endif  // >= VK_VERSION_1_1
  // clang-format off
  result_v1_0->robustBufferAccess                      = user_v1_0.robustBufferAccess && cvulkan_v1_0.robustBufferAccess;
  result_v1_0->fullDrawIndexUint32                     = user_v1_0.fullDrawIndexUint32 && cvulkan_v1_0.fullDrawIndexUint32;
  result_v1_0->imageCubeArray                          = user_v1_0.imageCubeArray && cvulkan_v1_0.imageCubeArray;
  result_v1_0->independentBlend                        = user_v1_0.independentBlend && cvulkan_v1_0.independentBlend;
  result_v1_0->geometryShader                          = user_v1_0.geometryShader && cvulkan_v1_0.geometryShader;
  result_v1_0->tessellationShader                      = user_v1_0.tessellationShader && cvulkan_v1_0.tessellationShader;
  result_v1_0->sampleRateShading                       = user_v1_0.sampleRateShading && cvulkan_v1_0.sampleRateShading;
  result_v1_0->dualSrcBlend                            = user_v1_0.dualSrcBlend && cvulkan_v1_0.dualSrcBlend;
  result_v1_0->logicOp                                 = user_v1_0.logicOp && cvulkan_v1_0.logicOp;
  result_v1_0->multiDrawIndirect                       = user_v1_0.multiDrawIndirect && cvulkan_v1_0.multiDrawIndirect;
  result_v1_0->drawIndirectFirstInstance               = user_v1_0.drawIndirectFirstInstance && cvulkan_v1_0.drawIndirectFirstInstance;
  result_v1_0->depthClamp                              = user_v1_0.depthClamp && cvulkan_v1_0.depthClamp;
  result_v1_0->depthBiasClamp                          = user_v1_0.depthBiasClamp && cvulkan_v1_0.depthBiasClamp;
  result_v1_0->fillModeNonSolid                        = user_v1_0.fillModeNonSolid && cvulkan_v1_0.fillModeNonSolid;
  result_v1_0->depthBounds                             = user_v1_0.depthBounds && cvulkan_v1_0.depthBounds;
  result_v1_0->wideLines                               = user_v1_0.wideLines && cvulkan_v1_0.wideLines;
  result_v1_0->largePoints                             = user_v1_0.largePoints && cvulkan_v1_0.largePoints;
  result_v1_0->alphaToOne                              = user_v1_0.alphaToOne && cvulkan_v1_0.alphaToOne;
  result_v1_0->multiViewport                           = user_v1_0.multiViewport && cvulkan_v1_0.multiViewport;
  result_v1_0->samplerAnisotropy                       = user_v1_0.samplerAnisotropy && cvulkan_v1_0.samplerAnisotropy;
  result_v1_0->textureCompressionETC2                  = user_v1_0.textureCompressionETC2 && cvulkan_v1_0.textureCompressionETC2;
  result_v1_0->textureCompressionASTC_LDR              = user_v1_0.textureCompressionASTC_LDR && cvulkan_v1_0.textureCompressionASTC_LDR;
  result_v1_0->textureCompressionBC                    = user_v1_0.textureCompressionBC && cvulkan_v1_0.textureCompressionBC;
  result_v1_0->occlusionQueryPrecise                   = user_v1_0.occlusionQueryPrecise && cvulkan_v1_0.occlusionQueryPrecise;
  result_v1_0->pipelineStatisticsQuery                 = user_v1_0.pipelineStatisticsQuery && cvulkan_v1_0.pipelineStatisticsQuery;
  result_v1_0->vertexPipelineStoresAndAtomics          = user_v1_0.vertexPipelineStoresAndAtomics && cvulkan_v1_0.vertexPipelineStoresAndAtomics;
  result_v1_0->fragmentStoresAndAtomics                = user_v1_0.fragmentStoresAndAtomics && cvulkan_v1_0.fragmentStoresAndAtomics;
  result_v1_0->shaderTessellationAndGeometryPointSize  = user_v1_0.shaderTessellationAndGeometryPointSize && cvulkan_v1_0.shaderTessellationAndGeometryPointSize;
  result_v1_0->shaderImageGatherExtended               = user_v1_0.shaderImageGatherExtended && cvulkan_v1_0.shaderImageGatherExtended;
  result_v1_0->shaderStorageImageExtendedFormats       = user_v1_0.shaderStorageImageExtendedFormats && cvulkan_v1_0.shaderStorageImageExtendedFormats;
  result_v1_0->shaderStorageImageMultisample           = user_v1_0.shaderStorageImageMultisample && cvulkan_v1_0.shaderStorageImageMultisample;
  result_v1_0->shaderStorageImageReadWithoutFormat     = user_v1_0.shaderStorageImageReadWithoutFormat && cvulkan_v1_0.shaderStorageImageReadWithoutFormat;
  result_v1_0->shaderStorageImageWriteWithoutFormat    = user_v1_0.shaderStorageImageWriteWithoutFormat && cvulkan_v1_0.shaderStorageImageWriteWithoutFormat;
  result_v1_0->shaderUniformBufferArrayDynamicIndexing = user_v1_0.shaderUniformBufferArrayDynamicIndexing && cvulkan_v1_0.shaderUniformBufferArrayDynamicIndexing;
  result_v1_0->shaderSampledImageArrayDynamicIndexing  = user_v1_0.shaderSampledImageArrayDynamicIndexing && cvulkan_v1_0.shaderSampledImageArrayDynamicIndexing;
  result_v1_0->shaderStorageBufferArrayDynamicIndexing = user_v1_0.shaderStorageBufferArrayDynamicIndexing && cvulkan_v1_0.shaderStorageBufferArrayDynamicIndexing;
  result_v1_0->shaderStorageImageArrayDynamicIndexing  = user_v1_0.shaderStorageImageArrayDynamicIndexing && cvulkan_v1_0.shaderStorageImageArrayDynamicIndexing;
  result_v1_0->shaderClipDistance                      = user_v1_0.shaderClipDistance && cvulkan_v1_0.shaderClipDistance;
  result_v1_0->shaderCullDistance                      = user_v1_0.shaderCullDistance && cvulkan_v1_0.shaderCullDistance;
  result_v1_0->shaderFloat64                           = user_v1_0.shaderFloat64 && cvulkan_v1_0.shaderFloat64;
  result_v1_0->shaderInt64                             = user_v1_0.shaderInt64 && cvulkan_v1_0.shaderInt64;
  result_v1_0->shaderInt16                             = user_v1_0.shaderInt16 && cvulkan_v1_0.shaderInt16;
  result_v1_0->shaderResourceResidency                 = user_v1_0.shaderResourceResidency && cvulkan_v1_0.shaderResourceResidency;
  result_v1_0->shaderResourceMinLod                    = user_v1_0.shaderResourceMinLod && cvulkan_v1_0.shaderResourceMinLod;
  result_v1_0->sparseBinding                           = user_v1_0.sparseBinding && cvulkan_v1_0.sparseBinding;
  result_v1_0->sparseResidencyBuffer                   = user_v1_0.sparseResidencyBuffer && cvulkan_v1_0.sparseResidencyBuffer;
  result_v1_0->sparseResidencyImage2D                  = user_v1_0.sparseResidencyImage2D && cvulkan_v1_0.sparseResidencyImage2D;
  result_v1_0->sparseResidencyImage3D                  = user_v1_0.sparseResidencyImage3D && cvulkan_v1_0.sparseResidencyImage3D;
  result_v1_0->sparseResidency2Samples                 = user_v1_0.sparseResidency2Samples && cvulkan_v1_0.sparseResidency2Samples;
  result_v1_0->sparseResidency4Samples                 = user_v1_0.sparseResidency4Samples && cvulkan_v1_0.sparseResidency4Samples;
  result_v1_0->sparseResidency8Samples                 = user_v1_0.sparseResidency8Samples && cvulkan_v1_0.sparseResidency8Samples;
  result_v1_0->sparseResidency16Samples                = user_v1_0.sparseResidency16Samples && cvulkan_v1_0.sparseResidency16Samples;
  result_v1_0->sparseResidencyAliased                  = user_v1_0.sparseResidencyAliased && cvulkan_v1_0.sparseResidencyAliased;
  result_v1_0->variableMultisampleRate                 = user_v1_0.variableMultisampleRate && cvulkan_v1_0.variableMultisampleRate;
  result_v1_0->inheritedQueries                        = user_v1_0.inheritedQueries && cvulkan_v1_0.inheritedQueries;
  #ifdef VK_VERSION_1_1
  result.list.features = *result_v1_0;
  #endif  // >= VK_VERSION_1_1
  // clang-format on

  //________________________________________________
  // Merge v1.1
#ifdef VK_VERSION_1_1
  // clang-format off
  result.v1_1.storageBuffer16BitAccess           = required.user.v1_1.storageBuffer16BitAccess && required.cvulkan->v1_1.storageBuffer16BitAccess;
  result.v1_1.uniformAndStorageBuffer16BitAccess = required.user.v1_1.uniformAndStorageBuffer16BitAccess && required.cvulkan->v1_1.uniformAndStorageBuffer16BitAccess;
  result.v1_1.storagePushConstant16              = required.user.v1_1.storagePushConstant16 && required.cvulkan->v1_1.storagePushConstant16;
  result.v1_1.storageInputOutput16               = required.user.v1_1.storageInputOutput16 && required.cvulkan->v1_1.storageInputOutput16;
  result.v1_1.multiview                          = required.user.v1_1.multiview && required.cvulkan->v1_1.multiview;
  result.v1_1.multiviewGeometryShader            = required.user.v1_1.multiviewGeometryShader && required.cvulkan->v1_1.multiviewGeometryShader;
  result.v1_1.multiviewTessellationShader        = required.user.v1_1.multiviewTessellationShader && required.cvulkan->v1_1.multiviewTessellationShader;
  result.v1_1.variablePointersStorageBuffer      = required.user.v1_1.variablePointersStorageBuffer && required.cvulkan->v1_1.variablePointersStorageBuffer;
  result.v1_1.variablePointers                   = required.user.v1_1.variablePointers && required.cvulkan->v1_1.variablePointers;
  result.v1_1.protectedMemory                    = required.user.v1_1.protectedMemory && required.cvulkan->v1_1.protectedMemory;
  result.v1_1.samplerYcbcrConversion             = required.user.v1_1.samplerYcbcrConversion && required.cvulkan->v1_1.samplerYcbcrConversion;
  result.v1_1.shaderDrawParameters               = required.user.v1_1.shaderDrawParameters && required.cvulkan->v1_1.shaderDrawParameters;
  // clang-format on
#endif  // >= VK_VERSION_1_1

  //________________________________________________
  // Merge v1.2
#ifdef VK_VERSION_1_2
  // clang-format off
  result.v1_2.samplerMirrorClampToEdge                           = required.user.v1_2.samplerMirrorClampToEdge && required.cvulkan->v1_2.samplerMirrorClampToEdge;
  result.v1_2.drawIndirectCount                                  = required.user.v1_2.drawIndirectCount && required.cvulkan->v1_2.drawIndirectCount;
  result.v1_2.storageBuffer8BitAccess                            = required.user.v1_2.storageBuffer8BitAccess && required.cvulkan->v1_2.storageBuffer8BitAccess;
  result.v1_2.uniformAndStorageBuffer8BitAccess                  = required.user.v1_2.uniformAndStorageBuffer8BitAccess && required.cvulkan->v1_2.uniformAndStorageBuffer8BitAccess;
  result.v1_2.storagePushConstant8                               = required.user.v1_2.storagePushConstant8 && required.cvulkan->v1_2.storagePushConstant8;
  result.v1_2.shaderBufferInt64Atomics                           = required.user.v1_2.shaderBufferInt64Atomics && required.cvulkan->v1_2.shaderBufferInt64Atomics;
  result.v1_2.shaderSharedInt64Atomics                           = required.user.v1_2.shaderSharedInt64Atomics && required.cvulkan->v1_2.shaderSharedInt64Atomics;
  result.v1_2.shaderFloat16                                      = required.user.v1_2.shaderFloat16 && required.cvulkan->v1_2.shaderFloat16;
  result.v1_2.shaderInt8                                         = required.user.v1_2.shaderInt8 && required.cvulkan->v1_2.shaderInt8;
  result.v1_2.descriptorIndexing                                 = required.user.v1_2.descriptorIndexing && required.cvulkan->v1_2.descriptorIndexing;
  result.v1_2.shaderInputAttachmentArrayDynamicIndexing          = required.user.v1_2.shaderInputAttachmentArrayDynamicIndexing && required.cvulkan->v1_2.shaderInputAttachmentArrayDynamicIndexing;
  result.v1_2.shaderUniformTexelBufferArrayDynamicIndexing       = required.user.v1_2.shaderUniformTexelBufferArrayDynamicIndexing && required.cvulkan->v1_2.shaderUniformTexelBufferArrayDynamicIndexing;
  result.v1_2.shaderStorageTexelBufferArrayDynamicIndexing       = required.user.v1_2.shaderStorageTexelBufferArrayDynamicIndexing && required.cvulkan->v1_2.shaderStorageTexelBufferArrayDynamicIndexing;
  result.v1_2.shaderUniformBufferArrayNonUniformIndexing         = required.user.v1_2.shaderUniformBufferArrayNonUniformIndexing && required.cvulkan->v1_2.shaderUniformBufferArrayNonUniformIndexing;
  result.v1_2.shaderSampledImageArrayNonUniformIndexing          = required.user.v1_2.shaderSampledImageArrayNonUniformIndexing && required.cvulkan->v1_2.shaderSampledImageArrayNonUniformIndexing;
  result.v1_2.shaderStorageBufferArrayNonUniformIndexing         = required.user.v1_2.shaderStorageBufferArrayNonUniformIndexing && required.cvulkan->v1_2.shaderStorageBufferArrayNonUniformIndexing;
  result.v1_2.shaderStorageImageArrayNonUniformIndexing          = required.user.v1_2.shaderStorageImageArrayNonUniformIndexing && required.cvulkan->v1_2.shaderStorageImageArrayNonUniformIndexing;
  result.v1_2.shaderInputAttachmentArrayNonUniformIndexing       = required.user.v1_2.shaderInputAttachmentArrayNonUniformIndexing && required.cvulkan->v1_2.shaderInputAttachmentArrayNonUniformIndexing;
  result.v1_2.shaderUniformTexelBufferArrayNonUniformIndexing    = required.user.v1_2.shaderUniformTexelBufferArrayNonUniformIndexing && required.cvulkan->v1_2.shaderUniformTexelBufferArrayNonUniformIndexing;
  result.v1_2.shaderStorageTexelBufferArrayNonUniformIndexing    = required.user.v1_2.shaderStorageTexelBufferArrayNonUniformIndexing && required.cvulkan->v1_2.shaderStorageTexelBufferArrayNonUniformIndexing;
  result.v1_2.descriptorBindingUniformBufferUpdateAfterBind      = required.user.v1_2.descriptorBindingUniformBufferUpdateAfterBind && required.cvulkan->v1_2.descriptorBindingUniformBufferUpdateAfterBind;
  result.v1_2.descriptorBindingSampledImageUpdateAfterBind       = required.user.v1_2.descriptorBindingSampledImageUpdateAfterBind && required.cvulkan->v1_2.descriptorBindingSampledImageUpdateAfterBind;
  result.v1_2.descriptorBindingStorageImageUpdateAfterBind       = required.user.v1_2.descriptorBindingStorageImageUpdateAfterBind && required.cvulkan->v1_2.descriptorBindingStorageImageUpdateAfterBind;
  result.v1_2.descriptorBindingStorageBufferUpdateAfterBind      = required.user.v1_2.descriptorBindingStorageBufferUpdateAfterBind && required.cvulkan->v1_2.descriptorBindingStorageBufferUpdateAfterBind;
  result.v1_2.descriptorBindingUniformTexelBufferUpdateAfterBind = required.user.v1_2.descriptorBindingUniformTexelBufferUpdateAfterBind && required.cvulkan->v1_2.descriptorBindingUniformTexelBufferUpdateAfterBind;
  result.v1_2.descriptorBindingStorageTexelBufferUpdateAfterBind = required.user.v1_2.descriptorBindingStorageTexelBufferUpdateAfterBind && required.cvulkan->v1_2.descriptorBindingStorageTexelBufferUpdateAfterBind;
  result.v1_2.descriptorBindingUpdateUnusedWhilePending          = required.user.v1_2.descriptorBindingUpdateUnusedWhilePending && required.cvulkan->v1_2.descriptorBindingUpdateUnusedWhilePending;
  result.v1_2.descriptorBindingPartiallyBound                    = required.user.v1_2.descriptorBindingPartiallyBound && required.cvulkan->v1_2.descriptorBindingPartiallyBound;
  result.v1_2.descriptorBindingVariableDescriptorCount           = required.user.v1_2.descriptorBindingVariableDescriptorCount && required.cvulkan->v1_2.descriptorBindingVariableDescriptorCount;
  result.v1_2.runtimeDescriptorArray                             = required.user.v1_2.runtimeDescriptorArray && required.cvulkan->v1_2.runtimeDescriptorArray;
  result.v1_2.samplerFilterMinmax                                = required.user.v1_2.samplerFilterMinmax && required.cvulkan->v1_2.samplerFilterMinmax;
  result.v1_2.scalarBlockLayout                                  = required.user.v1_2.scalarBlockLayout && required.cvulkan->v1_2.scalarBlockLayout;
  result.v1_2.imagelessFramebuffer                               = required.user.v1_2.imagelessFramebuffer && required.cvulkan->v1_2.imagelessFramebuffer;
  result.v1_2.uniformBufferStandardLayout                        = required.user.v1_2.uniformBufferStandardLayout && required.cvulkan->v1_2.uniformBufferStandardLayout;
  result.v1_2.shaderSubgroupExtendedTypes                        = required.user.v1_2.shaderSubgroupExtendedTypes && required.cvulkan->v1_2.shaderSubgroupExtendedTypes;
  result.v1_2.separateDepthStencilLayouts                        = required.user.v1_2.separateDepthStencilLayouts && required.cvulkan->v1_2.separateDepthStencilLayouts;
  result.v1_2.hostQueryReset                                     = required.user.v1_2.hostQueryReset && required.cvulkan->v1_2.hostQueryReset;
  result.v1_2.timelineSemaphore                                  = required.user.v1_2.timelineSemaphore && required.cvulkan->v1_2.timelineSemaphore;
  result.v1_2.bufferDeviceAddress                                = required.user.v1_2.bufferDeviceAddress && required.cvulkan->v1_2.bufferDeviceAddress;
  result.v1_2.bufferDeviceAddressCaptureReplay                   = required.user.v1_2.bufferDeviceAddressCaptureReplay && required.cvulkan->v1_2.bufferDeviceAddressCaptureReplay;
  result.v1_2.bufferDeviceAddressMultiDevice                     = required.user.v1_2.bufferDeviceAddressMultiDevice && required.cvulkan->v1_2.bufferDeviceAddressMultiDevice;
  result.v1_2.vulkanMemoryModel                                  = required.user.v1_2.vulkanMemoryModel && required.cvulkan->v1_2.vulkanMemoryModel;
  result.v1_2.vulkanMemoryModelDeviceScope                       = required.user.v1_2.vulkanMemoryModelDeviceScope && required.cvulkan->v1_2.vulkanMemoryModelDeviceScope;
  result.v1_2.vulkanMemoryModelAvailabilityVisibilityChains      = required.user.v1_2.vulkanMemoryModelAvailabilityVisibilityChains && required.cvulkan->v1_2.vulkanMemoryModelAvailabilityVisibilityChains;
  result.v1_2.shaderOutputViewportIndex                          = required.user.v1_2.shaderOutputViewportIndex && required.cvulkan->v1_2.shaderOutputViewportIndex;
  result.v1_2.shaderOutputLayer                                  = required.user.v1_2.shaderOutputLayer && required.cvulkan->v1_2.shaderOutputLayer;
  result.v1_2.subgroupBroadcastDynamicId                         = required.user.v1_2.subgroupBroadcastDynamicId && required.cvulkan->v1_2.subgroupBroadcastDynamicId;
  // clang-format on
#endif  // >= VK_VERSION_1_2

  //________________________________________________
  // Merge v1.3
#ifdef VK_VERSION_1_3
  // clang-format off
  result.v1_3.robustImageAccess                                  = required.user.v1_3.robustImageAccess && required.cvulkan->v1_3.robustImageAccess;
  result.v1_3.inlineUniformBlock                                 = required.user.v1_3.inlineUniformBlock && required.cvulkan->v1_3.inlineUniformBlock;
  result.v1_3.descriptorBindingInlineUniformBlockUpdateAfterBind = required.user.v1_3.descriptorBindingInlineUniformBlockUpdateAfterBind && required.cvulkan->v1_3.descriptorBindingInlineUniformBlockUpdateAfterBind;
  result.v1_3.pipelineCreationCacheControl                       = required.user.v1_3.pipelineCreationCacheControl && required.cvulkan->v1_3.pipelineCreationCacheControl;
  result.v1_3.privateData                                        = required.user.v1_3.privateData && required.cvulkan->v1_3.privateData;
  result.v1_3.shaderDemoteToHelperInvocation                     = required.user.v1_3.shaderDemoteToHelperInvocation && required.cvulkan->v1_3.shaderDemoteToHelperInvocation;
  result.v1_3.shaderTerminateInvocation                          = required.user.v1_3.shaderTerminateInvocation && required.cvulkan->v1_3.shaderTerminateInvocation;
  result.v1_3.subgroupSizeControl                                = required.user.v1_3.subgroupSizeControl && required.cvulkan->v1_3.subgroupSizeControl;
  result.v1_3.computeFullSubgroups                               = required.user.v1_3.computeFullSubgroups && required.cvulkan->v1_3.computeFullSubgroups;
  result.v1_3.synchronization2                                   = required.user.v1_3.synchronization2 && required.cvulkan->v1_3.synchronization2;
  result.v1_3.textureCompressionASTC_HDR                         = required.user.v1_3.textureCompressionASTC_HDR && required.cvulkan->v1_3.textureCompressionASTC_HDR;
  result.v1_3.shaderZeroInitializeWorkgroupMemory                = required.user.v1_3.shaderZeroInitializeWorkgroupMemory && required.cvulkan->v1_3.shaderZeroInitializeWorkgroupMemory;
  result.v1_3.dynamicRendering                                   = required.user.v1_3.dynamicRendering && required.cvulkan->v1_3.dynamicRendering;
  result.v1_3.shaderIntegerDotProduct                            = required.user.v1_3.shaderIntegerDotProduct && required.cvulkan->v1_3.shaderIntegerDotProduct;
  result.v1_3.maintenance4                                       = required.user.v1_3.maintenance4 && required.cvulkan->v1_3.maintenance4;
  // clang-format on
#endif  // >= VK_VERSION_1_3

  //________________________________________________
  // Merge v1.4
#ifdef VK_VERSION_1_4
  // clang-format off
  result.v1_4.globalPriorityQuery                    = required.user.v1_4.globalPriorityQuery && required.cvulkan->v1_4.globalPriorityQuery;
  result.v1_4.shaderSubgroupRotate                   = required.user.v1_4.shaderSubgroupRotate && required.cvulkan->v1_4.shaderSubgroupRotate;
  result.v1_4.shaderSubgroupRotateClustered          = required.user.v1_4.shaderSubgroupRotateClustered && required.cvulkan->v1_4.shaderSubgroupRotateClustered;
  result.v1_4.shaderFloatControls2                   = required.user.v1_4.shaderFloatControls2 && required.cvulkan->v1_4.shaderFloatControls2;
  result.v1_4.shaderExpectAssume                     = required.user.v1_4.shaderExpectAssume && required.cvulkan->v1_4.shaderExpectAssume;
  result.v1_4.rectangularLines                       = required.user.v1_4.rectangularLines && required.cvulkan->v1_4.rectangularLines;
  result.v1_4.bresenhamLines                         = required.user.v1_4.bresenhamLines && required.cvulkan->v1_4.bresenhamLines;
  result.v1_4.smoothLines                            = required.user.v1_4.smoothLines && required.cvulkan->v1_4.smoothLines;
  result.v1_4.stippledRectangularLines               = required.user.v1_4.stippledRectangularLines && required.cvulkan->v1_4.stippledRectangularLines;
  result.v1_4.stippledBresenhamLines                 = required.user.v1_4.stippledBresenhamLines && required.cvulkan->v1_4.stippledBresenhamLines;
  result.v1_4.stippledSmoothLines                    = required.user.v1_4.stippledSmoothLines && required.cvulkan->v1_4.stippledSmoothLines;
  result.v1_4.vertexAttributeInstanceRateDivisor     = required.user.v1_4.vertexAttributeInstanceRateDivisor && required.cvulkan->v1_4.vertexAttributeInstanceRateDivisor;
  result.v1_4.vertexAttributeInstanceRateZeroDivisor = required.user.v1_4.vertexAttributeInstanceRateZeroDivisor && required.cvulkan->v1_4.vertexAttributeInstanceRateZeroDivisor;
  result.v1_4.indexTypeUint8                         = required.user.v1_4.indexTypeUint8 && required.cvulkan->v1_4.indexTypeUint8;
  result.v1_4.dynamicRenderingLocalRead              = required.user.v1_4.dynamicRenderingLocalRead && required.cvulkan->v1_4.dynamicRenderingLocalRead;
  result.v1_4.maintenance5                           = required.user.v1_4.maintenance5 && required.cvulkan->v1_4.maintenance5;
  result.v1_4.maintenance6                           = required.user.v1_4.maintenance6 && required.cvulkan->v1_4.maintenance6;
  result.v1_4.pipelineProtectedAccess                = required.user.v1_4.pipelineProtectedAccess && required.cvulkan->v1_4.pipelineProtectedAccess;
  result.v1_4.pipelineRobustness                     = required.user.v1_4.pipelineRobustness && required.cvulkan->v1_4.pipelineRobustness;
  result.v1_4.hostImageCopy                          = required.user.v1_4.hostImageCopy && required.cvulkan->v1_4.hostImageCopy;
  result.v1_4.pushDescriptor                         = required.user.v1_4.pushDescriptor && required.cvulkan->v1_4.pushDescriptor;
  // clang-format on
#endif  // >= VK_VERSION_1_4

  //________________________________________________
  // Return the merged features list
  return result;
}  //:: cvk_device_features_create


void cvk_device_features_destroy (
  cvk_device_Features* const features
) {
  *features = cvk_device_features_empty();
}

