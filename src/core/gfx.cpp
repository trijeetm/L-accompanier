//--------------------------------------------------------------------------
// name: dm-gfx.cpp
// desc: gfx stuff
//
// author: Trijeet Mukhopadhyay (trijeetm@stanford.edu)
//   date: 2014
//--------------------------------------------------------------------------

#include "gfx.h"
#include "globals.h"
#include "y-entity.h"
#include "bk-sim.h"
#include "x-fun.h"

#include <stdlib.h>
using namespace std;

#define MAX_DRUMCUBES_ROWS      10

//-----------------------------------------------------------------------------
// function prototypes
//-----------------------------------------------------------------------------
bool gfx_init(int argc, const char ** argv);
void gfx_loop();
void help();
void initOpenGL();
void initSim();
void initGfx();
void idleFunc();
void displayFunc();
void reshapeFunc( GLsizei width, GLsizei height );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int button, int state, int x, int y );
void specialFunc( int key, int x, int y );

// width and height
long g_width = 1024;
long g_height = 720;
long g_last_width = g_width;
long g_last_height = g_height;

// global variables
GLboolean g_fullscreen = FALSE;

// gfx globals 
YText *g_tempo;
YCubeOutline *g_metronomeOutline;
YCube *g_metronome;
vector<YCube *> g_drumCubes[MAX_DRUMCUBES_ROWS];
YCubeOutline *g_intensitySliderOutline;
YCube *g_intensitySliderMarker;
vector<YCube *> g_bassNotes;
vector<YCubeOutline *> g_bassNotesOutlines;

// color scheme
Vector3D cream(1.0, 0.964, 0.898);
Vector3D seaGray(0.243, 0.27, 0.298);
Vector3D lGray(0.878, 0.872, 0.867);
Vector3D warmRed(1.0, 0.498, 0.4);
Vector3D blue(0.494, 0.807, 0.992);
// Vector3D lBlue(0.494, 0.807, 0.992);
Vector3D ourWhite( 1, 1, 1 );
Vector3D ourRed( 1, .5, .5 );
Vector3D ourBlue( 102.0f/255, 204.0f/255, 1.0f );
Vector3D ourOrange( 1, .75, .25 );
Vector3D ourGreen( .7, 1, .45 );
Vector3D ourGray( .4, .4, .4 );
Vector3D ourYellow( 1, 1, .25 );
Vector3D ourSoftYellow( .7, .7, .1 );
Vector3D ourPurple( .6, .25, .6 );

Vector3D g_drumCubeColors[3] = {cream, warmRed, blue};

//-----------------------------------------------------------------------------
// name: gfx_init()
// desc: GLUT Init
//-----------------------------------------------------------------------------
bool gfx_init( int argc, char ** argv ) {
    // initialize GLUT
    glutInit( &argc, argv );

    // print help
    help();
    
    // do our own initialization
    initGfx();
    // simulation
    initSim();
    
    return true;
}

//------------------------------------------------------------------
// initGfx()
// App gfx data init
//------------------------------------------------------------------
void initGfx() {
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( g_width, g_height );
    // set the window postion
    glutInitWindowPosition( 100, 100 );
    // create the window
    glutCreateWindow("L'accompanier");
    
    // set the idle function - called when idleFunc
    glutIdleFunc( idleFunc );
    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );
    // spec key
    glutSpecialFunc(specialFunc);

    // reset time
    XGfx::resetCurrentTime();
    // set simulation speed
    XGfx::setDeltaFactor( 1.0f );
    // get the first
    XGfx::getCurrentTime( true );
    // random
    XFun::srand();
    
    // set clear color
    glClearColor( 0, 0, 0, 1 );
    // enable color material
    glEnable( GL_COLOR_MATERIAL );
    // enable depth test
    glEnable( GL_DEPTH_TEST );
    // enable blending
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);

}

// // drum gfx init func
// void initDrumGfx() {
//     g_drumGfx = new YEntity();
//     // drum gfx
//     // grid outline
//     float yOffset = 2.25;
//     g_tutorial = new YEntity();
//     for (int i = 0; i < DM_N_GRID_ELEMENTS; i++) {
//         YCubeOutline *gridOutline = new YCubeOutline();
//         gridOutline->sca.setAll(1.0);
//         gridCoords[i].set(1.5 * ((i % (DM_N_GRID_ELEMENTS / 4)) + 0.5 - (DM_N_GRID_ELEMENTS / 8)), 
//             yOffset, 
//             0);
//         gridOutline->loc = gridCoords[i];
//         gridOutline->outlineColor = lGray;
//         gridOutline->lineWidth = 1.0;
//         g_drumGfx->addChild(gridOutline);
//         if (((i + 1) % (DM_N_GRID_ELEMENTS / 4)) == 0)
//             yOffset -= 1.5;
//         YText *helper = new YText();
//         helper->setCenterLocation(Vector3D(gridCoords[i].x + 0.45, gridCoords[i].y, gridCoords[i].z));
//         helper->col = lGray;
//         helper->setStretch(2);
//         switch (i) {
//             case 0:
//                 helper->set("1");
//             break;
//             case 1:
//                 helper->set("2");
//             break;
//             case 2:
//                 helper->set("3");
//             break;
//             case 3:
//                 helper->set("4");
//             break;
//             case 4:
//                 helper->set("q");
//             break;
//             case 5:
//                 helper->set("w");
//             break;
//             case 6:
//                 helper->set("e");
//             break;
//             case 7:
//                 helper->set("r");
//             break;
//             case 8:
//                 helper->set("a");
//             break;
//             case 9:
//                 helper->set("s");
//             break;
//             case 10:
//                 helper->set("d");
//             break;
//             case 11:
//                 helper->set("f");
//             break;
//             case 12:
//                 helper->set("z");
//             break;
//             case 13:
//                 helper->set("x");
//             break;
//             case 14:
//                 helper->set("c");
//             break;
//             case 15:
//                 helper->set("v");
//             break;
//         }
//         g_tutorial->addChild(helper);
//         keyOffLabels.push_back(helper);
//     }
//     g_drumGfx->addChild(g_tutorial);

//     // init playhead and trials
//     g_playhead = new YCubeOutline();
//     g_playhead->sca.setAll(1.1);
//     g_playhead->loc = gridCoords[Globals::playhead];
//     g_playhead->outlineColor = blue;
//     g_playhead->lineWidth = 5;
//     g_drumGfx->addChild(g_playhead);  

//     g_playheadTrail1 = new YCubeOutline();
//     g_playheadTrail1->sca.setAll(1.05);
//     g_playheadTrail1->loc = gridCoords[Globals::playhead];
//     g_playheadTrail1->outlineColor = blue;
//     g_playheadTrail1->lineWidth = 3;
//     g_drumGfx->addChild(g_playheadTrail1);  

//     // g_playheadTrail2 = new YCubeOutline();
//     // g_playheadTrail2->sca.setAll(1.01);
//     // g_playheadTrail2->loc = gridCoords[Globals::playhead];
//     // g_playheadTrail2->outlineColor = blue;
//     // g_playheadTrail2->lineWidth = 1;
//     // g_drumGfx->addChild(g_playheadTrail2);   

//     // init patch events
//     g_allPatchEvents = new YEntity();
//     for (int i = 0; i < DM_N_GRID_ELEMENTS; i++) {
//         for (int j = 0; j < DM_POLYPHONY; j++) {
//             YCube *cube = new YCube();
//             cube->sca.set(1.0, 1.0 / DM_POLYPHONY, 1);
//             cube->loc.set(gridCoords[i][0], 
//                 gridCoords[i][1] - 0.5 + (1.0 / (2 * DM_POLYPHONY)) + (j * (1.0 / (1 * DM_POLYPHONY))), 
//                 gridCoords[i][2]);
//             cube->col = patchColors[Globals::drumGrid[i].notes[j].patch];
//             cube->active = false;
//             g_patchCubes.push_back(cube);
//             g_allPatchEvents->addChild(cube);
//         }
//     }
//     g_drumGfx->addChild(g_allPatchEvents);

//     // init patches selector
//     g_allPatches = new YEntity();
//     g_drumLabels = new YEntity();
//     for (int i = 0; i < DM_POLYPHONY; i++) {
//         YCube *cube = new YCube();
//         cube->sca.set(1.0, 0.3, 1);
//         cube->loc.set(((i - (DM_POLYPHONY / 2)) * 1.2) + 0.6, 
//             3.5, 
//             0);
//         cube->col = patchColors[i];
//         g_patches.push_back(cube);
//         g_allPatches->addChild(cube);
//         // patch selector
//         YCubeOutline *cubeOutline = new YCubeOutline();
//         cubeOutline->active = false;
//         cubeOutline->sca.set(1.05, 0.35, 1.05);
//         cubeOutline->lineWidth = 2;
//         cubeOutline->loc.set(((i - (DM_POLYPHONY / 2)) * 1.2) + 0.6, 
//             3.5, 
//             0);
//         cubeOutline->outlineColor = seaGray;
//         g_patchSelector.push_back(cubeOutline);
//         g_allPatches->addChild(cubeOutline);
//         // patch label
//         YText *label = new YText();
//         switch (i) {
//             case 0:
//                 label->set("Bass 1 ( , )");
//             break;
//             case 1:
//                 label->set("Bass 2 ( . )");
//             break;
//             case 2:
//                 label->set("Snare 1 ( L )");
//             break;
//             case 3:
//                 label->set("Snare 2 ( ; )");
//             break;
//             case 4:
//                 label->set("Mid toms ( p )");
//             break;
//             case 5:
//                 label->set("High toms ( [ )");
//             break;
//             case 6:
//                 label->set("Hi Hat ( - )");
//             break;
//             case 7:
//                 label->set("Cymbal ( + )");
//             break;
//         }
//         label->col = seaGray;
//         Vector3D labelLoc(((i - (DM_POLYPHONY / 2)) * 1.2) + 0.6, 
//             3.75, 
//             0);
//         label->setCenterLocation(labelLoc);
//         g_drumLabels->addChild(label);
//     }
//     g_drumGfx->addChild(g_allPatches);
//     g_drumGfx->addChild(g_drumLabels);

//     YText *instrument = new YText();
//     instrument->col = seaGray;
//     instrument->set("Drums");
//     instrument->setStretch(3);
//     instrument->setCenterLocation(Vector3D(0, -3.25, 0));
//     g_drumGfx->addChild(instrument);

//     // add drum gfx to root
//     Globals::sim->root().addChild(g_drumGfx);
// }

void initMetronome() {
    g_metronomeOutline = new YCubeOutline();
    g_metronomeOutline->col = lGray;
    g_metronomeOutline->loc.set(0, -3.4, 0);
    g_metronome = new YCube();
    g_metronome->col = warmRed;
    g_metronome->loc.set(0, -3.4, 0);
    g_metronome->sca.setAll(0.2);
    g_metronomeOutline->sca.setAll(0.2);
    Globals::sim->root().addChild(g_metronomeOutline);
    Globals::sim->root().addChild(g_metronome); 
}

void initTempoDisplay() {
    g_tempo = new YText();
    g_tempo->loc.set(-0.5, -3.75, 0);
    g_tempo->col = cream;
    g_tempo->set("Tempo = 120");
    Globals::sim->root().addChild(g_tempo);
    YText *tempoUp = new YText();
    tempoUp->loc.set(-2.1, -3.95, 0);
    tempoUp->col = cream;
    tempoUp->set("<arrow-down> dec. tempo by 1");
    Globals::sim->root().addChild(tempoUp);
    YText *tempoDown = new YText();
    tempoDown->loc.set(0.25, -3.95, 0);
    tempoDown->col = cream;
    tempoDown->set("<arrow-up> inc. tempo by 1");
    Globals::sim->root().addChild(tempoDown);
}

void initDrumGfx() {
    // for (int j = 0; j < MAX_DRUMCUBES_ROWS; j++)
        for (int i = 0; i < Globals::nDrumTypes; i++) {
            g_drumCubes[0].push_back(new YCube());
            YCube *currCube = g_drumCubes[0][i];
            currCube->col = g_drumCubeColors[i];
            currCube->loc.set((i - (Globals::nDrumTypes / 2)) * 1.2, 0, 0);
            // currCube->active = false;
            // if (j == 0)
                // currCube->active = true;
            Globals::sim->root().addChild(currCube);
        }
}

void initIntensitySlider() {
    // outline
    g_intensitySliderOutline = new YCubeOutline();
    g_intensitySliderOutline->sca.set(3.5, 0.15, 0.15);
    g_intensitySliderOutline->col = cream;
    g_intensitySliderOutline->loc.set(0, 3.5, 0);
    Globals::sim->root().addChild(g_intensitySliderOutline);

    // slider
    g_intensitySliderMarker = new YCube();
    g_intensitySliderMarker->sca.set(0.35, 0.25, 0.25);
    g_intensitySliderMarker->col = warmRed;
    g_intensitySliderMarker->loc.set(0 - (3.5 / 2) + (0.35 / 2), 3.5, 0);
    Globals::sim->root().addChild(g_intensitySliderMarker);

    // label
    YText *intensityLabel = new YText();
    intensityLabel->set("Note Intensity");
    intensityLabel->setStretch(1.5);
    intensityLabel->col = cream;
    intensityLabel->setCenterLocation(Vector3D(0, 3.2, 0));
    Globals::sim->root().addChild(intensityLabel);
}

void initBassGfx() {
    for (int i = 0; i < 12; i++) {
        YCube *bassNote = new YCube();
        bassNote->col = blue;
        bassNote->sca.set(0.5, 0.2, 0.2);
        bassNote->loc.set(-((12 / 2) * 0.75) + (i * 0.75) + (0.75 / 2), -2, 0);
        g_bassNotes.push_back(bassNote);
        Globals::sim->root().addChild(bassNote);

        YCubeOutline *bassNoteOutline = new YCubeOutline();
        bassNoteOutline->col = blue;
        bassNoteOutline->sca.set(0.52, 0.22, 0.2);
        bassNoteOutline->loc.set(-((12 / 2) * 0.75) + (i * 0.75) + (0.75 / 2), -2, 0);
        bassNoteOutline->active = false;
        g_bassNotesOutlines.push_back(bassNoteOutline);
        Globals::sim->root().addChild(bassNoteOutline);
    }
}

//------------------------------------------------------------------
// initSim()
// Simulation init
//------------------------------------------------------------------
void initSim() {
    // instantiate simulation
    Globals::sim = new BKSim();

    // tempo
    initTempoDisplay();

    // metronome
    initMetronome();

    // drum gfx
    initDrumGfx();

    // intensity slider gfx
    initIntensitySlider();

    // init bass gfx
    initBassGfx();
}

//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void initOpenGL()
{
    // double buffer, use rgb color, enable depth buffer
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
    // initialize the window size
    glutInitWindowSize( g_width, g_height );
    // set the window postion
    glutInitWindowPosition( 100, 100 );
    // create the window
    glutCreateWindow("L'accompanier");
    
    // set the idle function - called when idleFunc
    glutIdleFunc( idleFunc );
    // set the display function - called when redrawing
    glutDisplayFunc( displayFunc );
    // set the reshape function - called when client area changes
    glutReshapeFunc( reshapeFunc );
    // set the keyboard function - called on keyboard events
    glutKeyboardFunc( keyboardFunc );
    // set the mouse function - called on mouse stuff
    glutMouseFunc( mouseFunc );
    
    // set clear color
    glClearColor( 0, 0, 0, 1 );
    // enable color material
    glEnable( GL_COLOR_MATERIAL );
    // enable depth test
    glEnable( GL_DEPTH_TEST );
    // enable blending
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);
}




//-----------------------------------------------------------------------------
// Name: reshapeFunc( )
// Desc: called when window size changes
//-----------------------------------------------------------------------------
void reshapeFunc( GLsizei w, GLsizei h )
{
    // save the new window size
    g_width = w; g_height = h;
    // map the view port to the client area
    glViewport( 0, 0, w, h );
    // set the matrix mode to project
    glMatrixMode( GL_PROJECTION );
    // load the identity matrix
    glLoadIdentity( );
    // create the viewing frustum
    gluPerspective( 5.0, (GLfloat) w / (GLfloat) h, 1.0, 300.0 );
    // set the matrix mode to modelview
    glMatrixMode( GL_MODELVIEW );
    // load the identity matrix
    glLoadIdentity( );
    // position the view point
    gluLookAt( 0.0f, 0.0f, 100.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
}




//-----------------------------------------------------------------------------
// Name: help( )
// Desc: print usage
//----------------------------------------------------------------------------- 
void help()
{
    cerr << "------------------------------------------------------------------------" << endl;
    cerr << "L'accompanier" << endl;
    cerr << "Trijeet Mukhopadhyay" << endl;
    cerr << "http://ccrma.stanford.edu/~trijeetm/l-accompanier" << endl;
    // cerr << "------------------------------------------------------------------------" << endl;
    // cerr << "'h' - print this help message" << endl;
    // cerr << "'`' - toggle fullscreen (below esc key)" << endl;
    // cerr << "'<esc>' - quit visualization" << endl;
    // cerr << "'<tab>' - switch instruments" << endl;
    // cerr << "left and right arrows to cycle b/w patches/notes" << endl;
    // cerr << "or use the following keys to select notes/patches" << endl;
    // cerr << "\t'-'\t'='\t" << endl;
    // cerr << "\t'p'\t'['\t" << endl;
    // cerr << "\t'l'\t';'\t" << endl;
    // cerr << "\t','\t'.'\t" << endl;
    // cerr << "and place them on the sequencer" << endl;
    // cerr << "keys correspond to position in the grid, hitting those toggles notes in that position" << endl;
    // cerr << "'<spacebar>' to clear all notes" << endl;
    // cerr << "<up-arrow> and <down-arrow> to control tempo" << endl;
    // cerr << "------------------------------------------------------------------------" << endl;
    // cerr << "PROTIP: if program crashes, run with wi-fi/bluetooth turned off" << endl;
    cerr << "------------------------------------------------------------------------" << endl;
}




//-----------------------------------------------------------------------------
// Name: keyboardFunc( )
// Desc: key event
//-----------------------------------------------------------------------------
void keyboardFunc( unsigned char key, int x, int y )
{
    switch( key )
    {
        case 27: // quit
            exit(1);
        break;
        case 'h': // print help
            help();
            break;          
        case '`': // toggle fullscreen
        {
            // check fullscreen
            if( !g_fullscreen )
            {
                g_last_width = g_width;
                g_last_height = g_height;
                glutFullScreen();
            }
            else
                glutReshapeWindow( g_last_width, g_last_height );
            
            // toggle variable value
            g_fullscreen = !g_fullscreen;
        }
        break;
        case 13: // toggle metronome
            Globals::toggleMetronome = !Globals::toggleMetronome;
        break;
        case '.': // dec drum gain
            Globals::drumGain -= 0.05;
            if (Globals::drumGain < 0)
                Globals::drumGain = 0;
        break;
        case '/': // inc drum gain
            Globals::drumGain += 0.05;
            if (Globals::drumGain > 1)
                Globals::drumGain = 1;
        break;
        case ';': // dec bass gain
            Globals::bassGain -= 0.05;
            if (Globals::bassGain < 0)
                Globals::bassGain = 0;
        break;
        case '\'': // inc bass gain
            Globals::bassGain += 0.05;
            if (Globals::bassGain > 1)
                Globals::bassGain = 1;
        break;
        case 's': // toggle scale build
            Globals::toggleScaleBuild = !Globals::toggleScaleBuild;
            if (!Globals::toggleScaleBuild) {
                cerr << endl << endl;
                cerr << "Scale:" << endl;
                for (int i = 0; i < 12; i++) {
                    cerr << Globals::scaleMap[i] << "\t";
                }
                cerr << endl;
                cerr << "Root:" << Globals::scaleRoot << endl;
            }
            if (Globals::toggleScaleBuild) {
                // reset scale
                Globals::scaleRoot = -1;
                for (int i = 0; i < 12; i++)
                    Globals::scaleMap[i] = false;
                // reset triads 
                Globals::triads.clear();
            }
        break;
    }
    
    // trigger redraw
    glutPostRedisplay( );
}

//-----------------------------------------------------------------------------
// Name: specialFunc( )
// Desc: handles arrow stuff
//-----------------------------------------------------------------------------
void specialFunc( int key, int x, int y )
{
    // check
    bool handled = false;
    
    // if not handled
    if( !handled )
    {
        switch( key )
        {
            case GLUT_KEY_LEFT:
                break;
            case GLUT_KEY_RIGHT:
                break;
            case GLUT_KEY_UP:
                if (Globals::bpm >= 400)
                    Globals::bpm == 400;
                else
                    Globals::bpm += 1;
                break;
            case GLUT_KEY_DOWN:
                if (Globals::bpm <= 1) 
                    Globals::bpm = 1;
                else
                    Globals::bpm += -1;
                break;
        }
    }
}


//-----------------------------------------------------------------------------
// Name: mouseFunc( )
// Desc: handles mouse stuff
//-----------------------------------------------------------------------------
void mouseFunc( int button, int state, int x, int y )
{
    if( button == GLUT_LEFT_BUTTON )
    {
        // when left mouse button is down
        if( state == GLUT_DOWN )
        {
        }
        else
        {
        }
    }
    else if ( button == GLUT_RIGHT_BUTTON )
    {
        // when right mouse button down
        if( state == GLUT_DOWN )
        {
        }
        else
        {
        }
    }
    else
    {
    }
    
    glutPostRedisplay( );
}


//-----------------------------------------------------------------------------
// Name: idleFunc( )
// Desc: callback from GLUT
//-----------------------------------------------------------------------------
void idleFunc( )
{
    // render the scene
    glutPostRedisplay( );
}

Vector3D g_sliderOffset(Globals::noteIntensity, Globals::noteIntensity, 0.2);

//-----------------------------------------------------------------------------
// Name: displayFunc( )
// Desc: callback function invoked to draw the client area
//-----------------------------------------------------------------------------
void displayFunc( )
{
    XGfx::getCurrentTime( true );

    if (Globals::toggleScaleBuild)
        glClearColor(lGray.x, lGray.y, lGray.z, 1.0);
    else 
        glClearColor(seaGray.x, seaGray.y, seaGray.z, 1.0);
    
    // clear the color and depth buffers
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    g_tempo->set("Tempo = " + to_string(Globals::bpm));

    // metronome
    g_metronome->active = true;
    if (Globals::beatCounter % (Globals::tSign.beatCount * 4) == 0)
        g_metronome->col = warmRed;
    else if (Globals::beatCounter % Globals::tSign.beatCount == 0)
        g_metronome->col = blue;
    else 
        g_metronome->active = false;

    // drums trigger
    for (int i = 0; i < Globals::nDrumTypes; i++) {
        YCube *currCube = g_drumCubes[0][i];
        if (Globals::drumGfx[i] == 1) {
            currCube->sca.set(1, 1.5, 1);
        }
        else
            currCube->sca.set(1, 1, 1);
    }

    // intensity update
    g_sliderOffset.goal = Globals::noteIntensity;
    g_sliderOffset.interp2(0.2);
    g_intensitySliderMarker->loc.set(0 - (3.5 / 2) + (0.35 / 2) + g_sliderOffset.value, 3.5, 0);

    // update bass display
    for (int i = 0; i < 12; i++) {
        g_bassNotesOutlines[i]->active = Globals::scaleMap[i];
        if (i == Globals::scaleRoot && Globals::toggleScaleBuild)
            g_bassNotes[i]->col = warmRed;
        else
            g_bassNotes[i]->col = blue;
        if (i == Globals::currentBassNote)
            g_bassNotes[i]->col = ourPurple;
    }

    // cascade simulation
    Globals::sim->systemCascade();

    // flush!
    glFlush( );
    // swap the double buffer
    glutSwapBuffers( );
}

//-----------------------------------------------------------------------------
// name: gfx_loop( )
// desc: hand off to graphics loop
//-----------------------------------------------------------------------------
void gfx_loop()
{
    // let GLUT handle the current thread from here
    glutMainLoop();
}