/* Eyes
 * Copyright (C) 2011  Krzysztof Mędrzycki, Damian Chiliński
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtGui>
#include <QApplication>

#include <iostream>
#include <string>
#include <libconfig.h++>

#include "eyes_window.hxx"
#include "debug.hxx"
#include "eyes.hxx"
#include "configurator.hxx"

using namespace std;
using namespace libconfig;

static const char help [] = {
    "usage: eyes [ARGUMENT [OPTION]]\n\n"
    "Arguments:\n"
    "\t--help -h\t\tPrint this text ang exit.\n"
    "\t--version -v\t\tPrint version and exit.\n"
    "\t--color=COLOR -c COLOR\tSet eyes color to COLOR.\n"
    "\t--config\t\tOpen configuration, then quit.\n"
    "\nSee --help colors for colors.\n"
};

static const char version [] = {
  "eyes Copyright (C) 2011  Krzysztof Mędrzycki, Damian, Chiliński\n"
  "This program comes with ABSOLUTELY NO WARRANTY.\n"
  "This is free software, and you are welcome to redistribute it\n"
  "under certain conditions.\n"
};

static const char help_colors [] = {
    "Avalible colors are:\n"
};

int eye_s,
    eye_m,
    eyes_w,
    eyes_h;

void on_destroy ();

int main ( int argc, char ** argv )
{
  QApplication app ( argc, argv );

  QStringList arg = app.arguments ();
  QString color = "";

  Config set;
  set.readFile ( "./config.cfg" );

  for ( int i = 0 ; i < arg.size () ; i++ )
  {
      if ( arg.at ( i ) == "-h" or arg.at ( i ) == "--help" )
      {
          if ( arg.size () > 2 )
          {
            if ( arg.at ( i+1 ) == "colors" )
            {
              cout << help_colors;
              Config cfg;
              try
              {
                cfg.readFile ( "colors.cfg" );
              }
              catch ( libconfig::ParseException e )
              {
                cout << e.what () << '\n' << e.getError () << '\n';
              }
              std::string color;
              int count;
              cfg.lookupValue ( "colors.count", count );
              cout << count << '\n';
              for ( int j=0 ; j<count ; j++ )
              {
                  cfg.lookupValue ( ( QString ( "colors.list[" ) + j + "]" ).toStdString (), color );
                  cout << "\t" << color << "\n";
              }
              exit ( 0 );
            }
          }
          cout << help;
          exit ( 0 );
      }
      else if ( arg.at ( i ) == "-v" or arg.at ( i ) == "--version" )
      {
          cout << version;
          exit ( 0 );
      }
      else if ( arg.at ( i ).split ( "=" )[0] == "--color" )
      {
          color = arg.at ( i ).split ( "=" )[1];
      }
      else if ( arg.at ( i ) == "-c" )
      {
          i++;
          color = arg.at ( i );
      }
      else if ( arg.at ( i ) == "--config" )
      {
          info << "(eyes_ui) starting eyes::configmode...\n";
          Configurator * conf = new Configurator ();
          conf->show ();
          return app.exec ();
      }
  }

  info << "(eyes_ui) starting eyes...\n";
  eyes_window win ( color );
  win.show ();

  return app.exec ();
}