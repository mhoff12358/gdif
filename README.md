# GDIF

GDIF is a Node tree based Dependency Injection framework for Godot.
It is built for GDExtension/Godot 4.0+.

## Adding to a project

Drop this into your project anywhere and run `scons target=debug` or whatever optimization level you're looking for.
The gdif.gdextension file will cause the Godot editor to include it.
Any other GDExtension libraries you write will want to link against it by including something like `env.Append(LIBS=["../gdif/bin/libgdif.windows.debug.x86_64"])`.

## Usage

The core building block of this library is the class `DIContext`. It allows you to register any `godot::Object*` (with an optional unique ID). Then it also allows you to lookup any registered instance by its type and ID (if no ID was provided, the empty string is used).

The static method `DIContext::get_nearest(Node*)` lets you find the nearest DIContext searching upwards though the given node's parents. In general this will be the root node of a scene, but not necessarily.

You can then write GDScript code like:
```
func _ready():
    var context = DIContext.get_nearest(self)

    var position = context.get_registered_node("Node2D")
    var sprite_position = context.get_registered_node_with_id("Node2D", "sprites")
    var animation_tree = context.get_registered_node("AnimationTree")
```

This lets you easily get access to other nodes within the scene without depending on specific NodePaths that could be changed later or have to be manually set up.

### Registration

Registration can be done manually by calling `DIContext::get_nearest` and then `DIContext::register_node`. This is generally done during `Node::_enter_tree`
 so that the registered node is available during all nodes' `_ready` method.

Another way to register nodes is to add a `DINodeRegistration` node as a child. Each `DINodeRegistration` will automatically register its parent during `_enter_tree`. In order to register multiple times with different IDs, multiple `DINodeRegistration` instances can be made.

### Registering GDScript instances

Because adding a GDScript to a node doesn't change its typename, they will by default be registered with type name of whatever node they're added to. This might not be what is desired, either because the script needs to be registered uniquely, or because it needs to register itself as an instance of a parent class.

There are 2 ways to override the type being registered. One is to pass in a type string to `DIContext::register_node_of_type` or the `DINodeRegistration`.

The other way is to add a `_di_name` method to the script, which replaces the class name as the default type name.

### Inheritance

#### Inherited lookups

The real value of the `DIContext`s is that they provide scopes with inheritance. Whenever looking up a registered value in a `DIContext`, if that type+ID does not exist locally it will also look into its parent, recursively all the way to the root context.

This means that you can have something in an enemy script like this:
```
func _ready():
    var context = DIContext.get_nearest(self)

    var position = context.get_registered_node("Node2D")
    var player_position = context.get_registered_node_with_id("Node2D", "player")
```
and now every enemy is able to have access to its own position provided by its own context, as well as the unique player's position that is provided by the root context.

#### Exporting registrations

The player scene in the previous example might have its own context, so that its `Node2D` can be easily referenced by its own scripts. And it would be confusing to have to register some values globally with the "player" ID but others locally without an ID.

The solution is to register everything inside the player scene with its local context, and then add a set of export rules to that local `DIContext` instance.

The `exported_registries` property of `DIContext` is an array of Type+IDs that should also be registered in the `DIContext`'s parent context. Each export also has optional type & ID overrides, and the Object* will be registered with these instead of the original type & ID.

In the case of the player scene, it's root position node might be a `CharacterBody2D`. It will want to register this locally as a `CharacterBody2D` with no ID. However, other scripts in other `DIContext`s are expecting a `Node2D`, a parent class of `CharacterBody2D`. And they're looking for the player's position not any other object's. So it makes the most sense to export this `CharacterBody2D` as a `Node2D` with the ID `"player"`.

## TODOs

Currently `DIContext` is a Node2D because I have a node tree that needs to maintain inheritance for Node2D's 2D transforms.

Ideally there should be a `DIContext` class that inherits from 'Object' and is provided by `DIContextNode`, `DIContextNode2D`, `DIContextNode3D`, etc. Then `DIContext::get_nearest` can find any of those nodes and get a `DIContext` instance from it.

Or there could be a GDScript method like `_di_context` that is expected to return a `DIContext` instance so that custom types don't need to be added as a parent node to an existing node tree.