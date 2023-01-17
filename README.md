# About BObject
BObject is a small and simple project based on GNOME GObject library. 
It was developed to learn about OOP implementation ic the C language, with a simple approach in mind.
BObject tries to mimic, in a much naive and simpler way, what GObject does behind the curtain. 

The BObject library consists of a Type System that defines types at runtime and a base object called BObject. 

The BObject library relies heavily on dynamic memory allocation, but it is possible to modify the Type System
implementation to work with fixed size arrays.


## Some Characteristics

 - Types are added to the Type System on demand, so no initializers are needed. 
 - All types inherit form BObject.
 - Definition of Public and Private fields (not Protected, sorry).
 - Single Inheritance, multiple interfaces.

## Some Lacking Characteristics
 
 - No type checking, relies on casting pointers.
 - "Object" type checking could be easily implemented, checking the object's type recursively among it's parents.
 - No interface type checks.

# Usage

## Declaring Types
BObject uses the same macros defined in GObject for Type definition and declaring. Reading [GObject Tutorial](https://docs.gtk.org/gobject/tutorial.html) 
is highly recommended.

By default, all object structures are open, user can add any public fields plus the parent's structure to achieve inheritance.
A constructor is needed for all new types

### Final (not derivable) types:
```
#define B_TYPE_LIST()		b_list_get_type()
B_DECLARE_FINAL_TYPE(BList, b_list, BObject)
/*
 * Class structure is hidden in B_DECLARE_FINAL_TYPE macro
 */
struct BList{
	BObject parent_instance;
	/*public fields*/
};
/*
 * Constructor
 */
BList* b_list_new( void );
```

### Derivable types:
```
#define B_TYPE_LIST()		b_list_get_type()
B_DECLARE_DERIVABLE_TYPE(BList, b_list, BObject)

struct BList{
	BObject parent_instance;
	/*public fields*/
};

struct BListClass{
    BObjectClasss parent_class;
    /* virtual methods*/
}

/*
 * Constructor
 */
BList* b_list_new( void );
```

## Defining Types
Type definition is done in source files (.c). 
Four functions need to be defined here:
 - Destructor
 - Object initializer
 - Object's Class initializer
 - Object's constructor

### Simple Type Definition:

```
B_DEFINE_TYPE(BList, b_list, B_TYPE_OBJECT())

static void b_list_class_initialize(BListClass *class)
{
	/*Implementation*/
	((BObjectClass*)class)->destructor = (void (*)(BObject*))b_list_destructor;
}

static void b_list_instance_initialize(BList *self)
{
	/*Implementation*/
}

static void b_list_destructor(BList *self)
{
	/*Implementation*/

    /* Call to parent's desctructor */
	BObjectClass* class = b_type_parent_class_get(type_id);
	class->destructor((BObject*)self);
		
}

BList* b_list_new(void){
    /*memory allocator, should always be used for type construction*/
	BList *obj = (BList*)b_object_new(b_list_get_type());
	return obj;
}

```

### Type Definition With Private Fields
```
/*
 * Private structure 
 */
typedef struct{
	/*private fields*/
}BListPrivate;

B_DEFINE_TYPE_WITH_PRIVATE(BList, b_list, B_TYPE_OBJECT())

static void b_list_class_initialize(BListClass *class)
{
	/*Implementation*/
	((BObjectClass*)class)->destructor = (void (*)(BObject*))b_list_destructor;
}

static void b_list_instance_initialize(BList *self)
{
	/*Implementation*/
	BListPrivate *priv = b_list_get_private(self);
    /*Do something with private fields*/
}

static void b_list_destructor(BList *self)
{
	/*Implementation*/

	BObjectClass* class = b_type_parent_class_get(type_id);
	class->destructor((BObject*)self);
		
}

BList* b_list_new(void){
	BList *obj = (BList*)b_object_new(b_list_get_type());
	return obj;
}
```
## Declaring Interfaces
Declaring and defining interfaces is done in a somewhat similar way as GObject with some minor differences.
The Type System treats interfaces as non instantiable types, interfaces don't have constructors nor destructors, just
virtual methods.
Interfaces do not interit from any parent class for now.

```
#define I_TYPE_OBSERVABLE()		i_observable_get_type()
B_DECLARE_INTERFACE(IObservable, i_observable)

struct IObservableInterface{
	/*virual methods*/
	void (*subscribe)(IObservable*,IObserver*);
	void (*unsubscribe)(IObservable*,IObserver*);
};

/*Interface Methods*/
void i_observable_subsribe(IObservable *publisher, IObserver *subscriber);
void i_observable_unsubsribe(IObservable *publisher, IObserver *subscriber);

```

## Defining Interfaces
```
B_DEFINE_INTERFACE(IObservable, i_observable)

/*Interface Methods Implementation*/

void i_observable_subsribe(IObservable *publisher, IObserver *subscriber)
{
    IObservableInterface *iface = i_observable_get_iface(publisher);
    iface->subscribe(publisher, subscriber);
}
void i_observable_unsubsribe(IObservable *publisher, IObserver *subscriber)
{
    IObservableInterface *iface = i_observable_get_iface(publisher);
    iface->unsubscribe(publisher, subscriber);
}
```

## Defining Types Implementing Interfaces
To define a type that implement interfaces the B_DEFINE_TYPE_EXTENDED macro is used. 
Three more macros are used to define interface implementation and overriding:
 - B_INTERFACE
 - B_IMPLEMENT_INTERFACES
 - B_OVERRIDE_INTERFACE

The first macro, B_INTERFACE, is used to define the type of the interface and the interface initializer function.
Is the base macro used for this purpose and B_IMPLEMENT_INTERFACES and B_OVERRIDE_INTERFACE use it.

The second macro, B_IMPLEMENT_INTERFACE, is a variadic macro that accepts multiple B_INTERFACE arguments and **adds/implements** interfaces to the class. 
This macro is used to define the interfaces that a base class implements. Derived classes must not use this macro to override inherited
interfaces, as this macro adds. 

The third macro, B_OVERRIDE_INTERFACE, is a macro that accepts one B_INTERFACE argument and overrides an already implemented interface. 
Derived classes should use this macro to override an inherited interface. Must never be used to **implement** new interfaces. 

***Note: B_IMPLEMENT_INTERFACES and B_OVERRIDE_INTERFACE have different definitions and are inconsistent. This will be fixed in future releases.***

### Interface Implementation
```
static void i_observer_interface_init(IObserverInterface *iface);
static void i_drawable_interface_init(IDrawableInterface *iface);

B_DEFINE_TYPE_EXTENDED(BSubscriber, b_subscriber, B_TYPE_OBJECT(),
	B_IMPLEMENT_INTERFACES(
		B_INTERFACE(I_TYPE_OBSERVER(), i_observer_interface_init)
        B_INTERFACE(I_TYPE_DRAWABLE(), i_drawable_interface_init)
    )
)

static void i_observer_interface_init(IObserverInterface *iface)
{
	/*Implementation*/
}

static void i_drawable_interface_init(IDrawableInterface *iface)
{
	/*Implementation*/
}
```
### Interface Overriding
```
static void i_observer_interface_init(IObserverInterface *iface);
static void i_drawable_interface_init(IDrawableInterface *iface);

B_DEFINE_TYPE_EXTENDED(BSubscriber, b_subscriber, B_TYPE_OBJECT(),
	B_OVERRIDE_INTERFACE(
		B_INTERFACE(I_TYPE_OBSERVER(), i_observer_interface_init)
    )
    B_OVERRIDE_INTERFACE(
		B_INTERFACE(I_TYPE_DRAWABLE(), i_drawable_interface_init)
    )
)

static void i_observer_interface_init(IObserverInterface *iface)
{
	/*Implementation*/
}

static void i_drawable_interface_init(IDrawableInterface *iface)
{
	/*Implementation*/
}
```
### B_DEFINE_TYPE_EXTENDED Extra Usage
You can use B_DEFINE_TYPE_EXTENDED to define a type that implements interfaces and has private data with B_DEFINE_PRIVATE macro.
```
static void i_observer_interface_init(IObserverInterface *iface);

typedef struct{
    /* Private Fields */
}BSubscriberPrivate

B_DEFINE_TYPE_EXTENDED(BSubscriber, b_subscriber, B_TYPE_OBJECT(),
	B_DEFINE_PRIVATE(BSubscriber, b_subscriber)
    B_IMPLEMENT_INTERFACES(
		B_INTERFACE(I_TYPE_OBSERVER(), i_observer_interface_init)
        B_INTERFACE(I_TYPE_DRAWABLE(), i_drawable_interface_init)
    )
)

static void i_observer_interface_init(IObserverInterface *iface)
{
	/*Implementation*/
}
```

# BObject Theory
Theory about BObject can be found  [here](docs/BObject_structure.md)

# Contributing

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement". Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (git checkout -b feature/AmazingFeature)
3. Commit your Changes (git commit -m 'Add some AmazingFeature')
4. Push to the Branch (git push origin feature/AmazingFeature)
5. Open a Pull Request