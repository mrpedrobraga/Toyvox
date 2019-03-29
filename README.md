# Toyvox

Toyvox is very unique, because it actually goes against the predominant way of making 3d games, that we are all used to.

Toyvox is a flat-shaded voxel engine, that's easy and simple to use, and yet, very powerful.
It is a *C++* library that uses OPENGL to render, so you can import it and use it on your favourite IDE. Or in the command line, as well.

The flat shaded approach is very rare for now, and it's kinda of misunterstood.
The idea of flat shading is: voxels are elementary. So they can only hold one color.

Most of the current voxel engines render the voxels as cubes, with different faces and smooth lighting.
That's not the objective of Toyvox.

There are, however, some examples I can point to.

Like the game Voxelnauts - and the amazing arts made by [Zach Soares](https://www.zsinked.com/) for that game.

Also, there are other smaller artists that use the flat-shading, because there is a list of benefits you get by doing it:

* You can easily make whatever shape you want. With flat-shading, voxelart gets closer to the pixelart side, than to the 3d - that's the objective of this engine. You can highlight the shape, and use the colors at your favour. Instead of shading, you can also make lighting, and make objects like fire and particles look great. Those voxel models look a lot more like 3d pixelart.

![For example, this escape pod by y2bcrazy at imgur](https://i.imgur.com/HYzIikB.gif|width=100)

Also, you can animate it however you want! For example, you can animate it like pixelart (having various frames that the animation renders trough), or even have small models connected together trough *model points*. Both types of animation can be done with this engine.

![I mean, just look at this! Animated by Zach Soares.](https://i.pinimg.com/originals/47/04/97/470497843635a32fb5ecb9bd068a5310.gif)

### And it supports high-voxel models as well!

With those, the auto-lighting extension will look even better! The voxel approach, makes it possible to make high detailed models, with smooth color transitions. (If low-voxel is pixelart, high voxel is an HD image! And HD images are dope!)

Also, another technique to hide the voxels, derives from the world of pixels as well - antialiasing! Translucent voxels in the spaces where the model should be partially occupying. This is something never attempted before, and yet amazing!

### Here's a list of models that rendered with prototypes of the engine!

Ok, so those models we're made and rendered in Java. The first in pure Java, the second in LWJGL.
But although they are prototypes, they express what the engine will look like, visually.

Also, the actual engine is going to be orders of magnitude more powerful than it's prototypes!

#### This one was rendered by @Yoris1
![Dope](https://i.gyazo.com/fd778387e6c269da87fe87d4aebcfff6.gif|width=200)

#### This is my version of it
![Every voxel is transferred to a pixel, actually!](https://i.gyazo.com/6dc483c0f3dd406b2d59136daceea31c.gif)

#### This is a Low-Voxel model. Also by @Yoris1
![Another antisphere](https://media.discordapp.net/attachments/557838269648338944/560217648202514462/unknown.png?width=400&height=227)

#### Yes! We love *antispheres*! It's our logo! This is the original one:
![Original Antisphere! And I made it!](https://media.discordapp.net/attachments/557838269648338944/557840245945794580/JPEG_20190319_082404.jpg?width=300&height=300)
