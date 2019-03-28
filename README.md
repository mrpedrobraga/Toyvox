# Toyvox

Hello! So you want to create a game, and you're looking for the engine
that best suits it!

Well, Toyvox Engine is a very unique type of voxel engine, because it's very
similar to 2d (pixel) engines, on it's design. The models of the objects
are expressed in animations that contain frames. Those frames are voxel sprites.

If you aren't familiarized, a voxel (stands for volume pixel) is an elementary
part of a model. A little cube that can hold one color. Just like a 3d pixel, right?

By default, there's no lighting on Toyvox so the models may look flat at first, but
that just means you have can more freedom with your own shading. (I mean, of course the
model will look flat with just one voxel color!)

However, you'll be able to add lighting easily. Just add a lamp and set up the perfect
mood for your game!

On the next section, we're going to explain how the engine works on its core
so you can have an idea of what you're getting into.

#Objects and Object Types

Object Types are the classes, and Objects are their instances.
That's how them are going to be referenced from now on. 
So when you see "Object Type" we're talking about all the objects
of the same TYPE. When you see "Object", we are talking about an instance 
of said type.

#Layout and Hierarchy

There's not much hierarchy on Toyvox, apart from the organization of the world.
Your World may have many layouts, that are instances of Layout3D.
Layouts have Objects that are sorted by layer and WOrder.
Their position on the layout doesn't matter, just their UNIQUE IDs,
that are assigned when they're created.

There is the possibility to create Object Types. They're an extension of the default
Object Type, but they can have unique instance variables.
Instance variables are shared by Object Types, but they hold an unique value for
each instance of the type. That's useful for creating Health, Ammo, etc.

Objects can be removed from one Layout and added to another, keeping it's properties,
but this time, relative to the other layout.
For example, if you remove a monster with position (4, 3, 5) it will enter the other
layout with the same position. So make sure to explicitly change it if you want.

#Parenting

Objects can also be placed into object groups, that can be moved by themselves.
Their position now it relative to the group.

#Eventing System

Toyvox will have a structure of events that have actions that are called when their
conditions are true. For each reachable event, it tests: if its condition is true,
and if that condition is in trigger mode, if it was true last tick.

For example:

A Is overlapping B -> print("Overlapping")

will spam "Overlapping" on the exit, when object type A is overlapping object type B.
By changing that event to Trigger mode, you get something like.

When A collides with B -> print("Overlapping")

This event will only trigger it's actions when it's condition is true,
but will only run once.

The eventing aspect guarantees that the System will be able to manipulate the objects.
They objects, don't contain any piece of logic, just their unique properties.
They're more like toys, puppets that the system can play with.

Also, there will be default functions to test specific conditions, so you won't have to make all of them for yourself.
Some conditions like "is A overlapping B" are needed in many games, so we'll make them on a library.

To setup an event, you can create functions and set the event's condition and actions to pointers to them
(You will probably make a file just for a certain type of events -- an eventing sheet!)

#Eventing Groups

Toyvox will allow to create Eventing Groups, so you can stack objects that are similar.
Any events that references an Eventing Group, references all of it's children object types.
