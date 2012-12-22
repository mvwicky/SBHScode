This library contains a class that controls the robots for the SBHS robotics team's robots.
Some Notes:
Make sure to include the distance macros at the beginning of all files that use the create.
like this:
"#define DIST100(x) ( ( (.952847 * x) - 26.9) : (x) )
#define DIST200(x) ( ( (.971316 * x) - 5.06716) : (x) )
#define DIST300
#define DIST500
#define DIST600
#define DIST700
#define DIST800
#define DIST900"