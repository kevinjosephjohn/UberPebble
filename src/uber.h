
static Window *window; // Category Window



static GRect bounds;

const bool animated = true; 
char *msg;

char *names[4];
char *estimates[4];


//Splash 
static Window *splash_window; // Splash Window

GBitmap *splash;
static BitmapLayer *image_layer;
AppTimer *timer;


//results
static Window *r_window; //Results Window
static SimpleMenuLayer *r_menu_layer; //Result Menu
static SimpleMenuItem r_items[10]; //Holds result items
static SimpleMenuSection r_sections[1]; //Holds results sections