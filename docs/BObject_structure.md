# BObject Structure
This section describes the implementation behind BObject and the Type System.
## Type System
The Type System consists of an array of TypeNodes that contain the type's information in the form of struct fields. 
These fields describe the type's ID, size, class' size, interfaces, private structure size, private offset, class initializer
and object initializer.   

The Type System is in charge of creating and initializing object instances, creating and initializing classes, registering
interfaces and providing inheritance mechanisms. The Type System is also in charge of deleting objects and cleaning the memory used
by itself.

## Basic OOP concepts in the C language
Some basic concepts of the OOP paradigm can be easily implemented in the C language. There are some good OOP in C guides on the internet.

OOP is achieved by using `struct` for data encapsulation and functions that accept `struct*` as parameters for class methods as follows:
```
/*A simple 2D vector object example*/
/* vector2d.h */

/* Struct representing the object data */
struct Vector2D{
    int x;
    int y;
};

/* Class methods */
int vector_2d_get_magnitude(struct Vector2D *v);
int vector_2d_normalize(struct Vector2D *v);
int vector_2d_add_vector_2d(struct Vector2D *v1, struct Vector2D *v2);
```
This Vector2D struct represents the Vector2D class, with public data and public methods.
Data hiding, "private" fields, can be achieved by defining the struct inside the source file.

Header file vector2d.h:
```
/*A simple 2D vector object example*/
/* vector2d.h */

/* Struct representing the object data */
struct Vector2D;

/* Class methods */
int vector_2d_get_magnitude(struct Vector2D *v);
int vector_2d_normalize(struct Vector2D *v);
int vector_2d_add_vector_2d(struct Vector2D *v1, struct Vector2D *v2);
```

Source file vector2d.c
```
/* vector2d.c */

/* Struct representing the object data */
struct Vector2D{
    int x;
    int y;
};

/* Class methods */
int vector_2d_get_magnitude(struct Vector2D *v)
{
    /*Implementation*/
}
...
```
Inheritance can also be achieved easily. The simplest way of doing this is by embedding the parent's structure in the derived object's structure:
```
/*parent.h*/
struct parent{
    int a, b, c;
};

/*child.h*/
struct child{
    struct parent parent_object;
    int x, y, z;
};
```

Lastly, polymorphism can also be achieved by defining a class structure with virtual methods. Note that inheritance also applies with class structures.
For a graphics library something like this could be implemented:
```
/* figure.h */
struct FigureClass;

struct Figure{
    color_t color;
    int x,y;
    //A reference to the object's class is needed, so it's just easier to palce it here.
    FigureClass *class;
}

struct FigureClass{
    void (*draw)(Figure*); //here we define the virtual method that derived classes need to implement.
}

/* Provide the implementation of the virtual method to achieve polymorphism */
void figure_draw(Figure *fig);


/* figure.c */

//Implementation of the method in the parent class
//note that this is the default method implementation and can be NULL if the class is "abstract" (not instantiable)"
static void _figure_draw(Figure *fig)
{
    /*Do something or nothing*/
}

void figure_draw(Figure *fig)
{
    fig->class->draw(fig);//here we are accessing the objects draw implementation through it's class structure.
    //this implies that every Figure Object will need to initialize its class pointer through some mechanism.
}

/*One way of many of initializing the class pointer*/
static FigureClass class = (FigureClass){
    .draw = _figure_draw
};

static void _figure_initialize(Figure *fig){
    fig->class = &class;
}

``` 

So it's possible and easy to implement basic OOP concepts int the C language. But what about interfaces, private **and** public data, etc?
We need a Type System to achieve this, and is something that GNOME's GObject library does and BObject took inspiration from.

**Note1:** BObject implementation was designed as simple as possible, it does not implement full OOP as expected from a 
fully OOP oriented language as Java or C#. many things were left behind (Properties, and many many others). Although I 
think that it's possible to extend the Type System to include some of these features.

**Note2:** BObject does not copy the GObject's implementation.

## Public and Private Fields
How to implement both public and private data for an object structure? The former implementation of data encapsulation makes it
impossible to define an structure with both private and public data due to the C languages restrictions.

Another implementation is needed and this is something that GObject solved by having an additional "private" structure defined in the object's
source file. So, how do this work on BObject? Some mechanism is needed to "join" both structures as one, and this is achieved 
by adding both struct sizes. 

We can visualize what's happening with this diagram:
This represents the memory block of the "public" struct
|++++++++|
|++++++++|

and this represents the memory block of the "private" struct
|........|
|........|
|........|

To join them, we just need to add their sizes, then the Type System will allocate a memory block with the total size, creating a block like this:
|        |
|        |
|        |
|        |
|        |

Then we need to provide an offset to divide the two structs. BObject always divide the block placing the private data "above" the public data.
This helps with inheritance of both public and private data. Note that private data "cannot" be inherited, as the private struct is defined
in the source file, nobody can access the struct definition.
The allocated memory block will be seen as this by the Type System:
|........|
|........|
|........|----> here's the offset value
|++++++++|
|++++++++|

When creating an object, the returned pointer will be offset a positive amount to point to the public data. When accessing 
the private data the object's pointer will be offset by a negative amount to be placed at the origin of the block, 
the position of the private data.
This offset is provided by the Type System when registering a new type with private data. A getter function for the private data is generated by the Type System macros B_DEFINE_...

## Interfaces and Class Extensions


## Inheritance and Interface Implementation

