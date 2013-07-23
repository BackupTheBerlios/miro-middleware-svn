// -*- c++ -*- ///////////////////////////////////////////////////////////////
//
// This file is part of Miro (The Middleware for Robots)
// Copyright (C) 1999-2005
// Department of Neuroinformatics, University of Ulm, Germany
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// $Id$
//
//

#include "VideoControl.h"
#include "VideoControlDialog.h"

#include "miro/Client.h"
#include "idl/VideoControlC.h"

#include <qapplication.h>
#include <qcombobox.h>

#include <string>
#include <vector>


using namespace Miro;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Miro::Client client(argc, argv);

  // possible camera control interface names
  std::vector<std::string> cameraControlChoice;
  cameraControlChoice.push_back("CameraControlUnicap");
  cameraControlChoice.push_back("CameraControl1394");
  cameraControlChoice.push_back("CameraControlQuickCam");

  // look for available ones
  std::vector<std::string> cameraControlName;
  for (std::vector<std::string>::const_iterator i=cameraControlChoice.begin(); i!=cameraControlChoice.end(); ++i) {
    try {
      Video::CameraControl_var video_control_ = client.resolveName<Video::CameraControl>(i->c_str());
      cameraControlName.push_back(*i);
    } catch (...) {
    }
  }

  if (cameraControlName.size() == 0) {
    std::cout << std::endl;
    std::cout << "No valid camera control interface found." << std::endl;
    std::cout << "You've probably forgot to choose the correct corba namespace" << std::endl;
    std::cout << std::endl;
    return 0;
  }

  // create dialog to let the user choose
  QString name;
  VideoControlDialog *dlg = new VideoControlDialog(0, QString::null, 0, TRUE);
  for (std::vector<std::string>::const_iterator i=cameraControlName.begin(); i!=cameraControlName.end(); ++i)
    dlg->comboBox->insertItem(*i);
  if (dlg->exec() == QDialog::Accepted) {
    name = dlg->comboBox->currentText();
  }
   
  // start control window
  VideoControl * dialog = new VideoControl(client, NULL, name);

  a.setMainWidget(dialog);
  dialog->setCaption("QtVideoControl - " + name);
  dialog->show();

  return a.exec();
}




