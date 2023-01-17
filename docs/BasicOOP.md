# Basic OOP Concepts in C
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

So it's possible to implement basic OOP concepts int the C language. But what about interfaces, private **and** public data,
protected members, properties, etc?
We need a Type System to achieve some of these features, and is something that GNOME's GObject library does and BObject 
took inspiration from.