## Queue Families -> Queue Groups / Types
> https://registry.khronos.org/vulkan/specs/latest/man/html/vkGetDeviceQueue.html
> **vkGetDeviceQueue**:
> `queueFamilyIndex` is the index of the queue family to which the queue belongs.
> `queueIndex` is the index within this queue family of the queue to retrieve.

Queue families are oddly named.
They are more like Types of Queues grouped together.

Each family has 1 or more queues in them,
and all queues in a family support identical operations.

Think of it as a nested array.
There are a number of queue families, each with a number of queues.

The thing about it is that you cant just create queues from arbitrary queue families
because each family supports different operations.
Some are compute & transfer, some do compute graphics and transfer, some do video encode/decoding work, etc etc.

Typically, its easiest to use a single queue from each family,
as most hardware only has 1-2 queues per family.

The reason to use more than one queue in a single family
is for multi threaded queue submission
since submitting to a queue can only be done from one thread at a time.

But those don’t yield performance gains just by using them,
as the bottleneck is rarely in submission overhead with vulkan.

