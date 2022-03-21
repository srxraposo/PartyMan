// begin include guard
#ifndef TEXTUREREGIONS_BACKGROUND
#define TEXTUREREGIONS_BACKGROUND

// define names for single regions
#define RegionWall  0
#define RegionPlayer  1
#define RegionPellet 2
#define RegionScoreFrame 3
#define RegionStart 4
#define RegionYouWin 5
#define RegionZero 6

#define DigitWidth 7


// your initialization code must call this function
void DefineRegions_background() 
{
    select_texture( background );

    // define single regions
    select_region( RegionWall );
    define_region( 1,1,  13,13,  1,1 );
    select_region( RegionPlayer );
    define_region( 18,1,  26,9,  18,1 );
    select_region( RegionPellet );
    define_region( 31,4,  35,8,  31,4 );
    select_region( RegionScoreFrame );
    define_region( 0,31,  95,69,  0,31 );
    select_region( RegionStart );
    define_region( 1,74,  641,434,  1,74 );
    select_region( RegionYouWin );
    define_region( 1,437,  641,797,  1,437 );

    define_region_matrix( RegionZero, 0,18,  9,30,  0,18,  10,1,  0 );

}

// end include guard
#endif
