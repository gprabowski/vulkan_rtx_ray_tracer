### separate queue for transfer operations only

- [ ] modify QueueFamilyIndices and findQueueFamilies to explicitly \
look for a queue family with the VK\_QUEUE\_TRANSFER\_BIT, but not \
the VK\_QUEUE\_GRAPHICS\_BIT
- [ ] modify createLogicalDevice to request a handle to the transfer \
queue
- [ ] create a second command pool for command buffers that are submitted \
on the transfer queue family
- [ ] change the sharingMode of resources to be VK\_SHARING\_MODE\_CONCURRENT \
and specify both the graphics and transfer queue families
- [ ] submit any transfer commands like vkCmdCopyBuffer to the transfer queue \
insted of the graphics queue

### aliasing of index buffers instead of separate ones
### switch to push constants
### move commands to a single buffer 
- [ ] especially transitions and copy in the createTextureImage function
- [ ] create a setupCommandBuffer (helper functions record commands into it)
- [ ] next flushSetupCommands (execture the commands that have been
recorded so far
