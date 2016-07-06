/**
 * This file is part of the libndef project.
 *
 * Copyright (C) 2011, Romuald Conty
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

//apt-get install qt5-default
//LD_LIBRARY_PATH=/home/radix/programs/libndef/libndef ./ndef-decode ndef
//LD_LIBRARY_PATH=/home/radix/programs/libndef/libndef ./ndef-encode -t "Test1" "en-US" > ndef

//For custom test
//LD_LIBRARY_PATH=../../extras/libndef/libndef ./snep-encode -z test.txt "en-US" > ndef1
#include "snep-encode.h"
#include <QtCore/QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <QFile>
 
#include <ndef/ndefmessage.h>


QTextStream out(stdout);
QTextStream err(stderr);
QTextStream info(stderr);

QFile output("receipt_nfc");

typedef QList<NDEFRecord> NDEFRecordList;
//typedef QList<char> SNEPFrame;

typedef union snepLenUnion
{
	int lenth4byte;
	char byte[4];
};

typedef enum snepRequest
{
	snepRequestContinue = 0x00, 
	snepRequestGet = 0x01, 
	snepRequestPut = 0x02, 
	snepRequestReject = 0x7f
};

QString openFile(char *dataFilePath)
{
#if 1
	QFile file(dataFilePath);
	QString string;
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qDebug() << "Wasn't ready:";
		file.error();
	}
	else
	{
		//qDebug() << "ndef file ready:";
		QTextStream in(&file);
		qint64 size = file.size();
		while ( !in.atEnd() )
		{
			QString line = in.readLine();
			qDebug() << "message: " << line;
			string += line;
		}
		qDebug() << "File length : "<< size;
	}
	return string;
#endif
}

//create snep frame
QByteArray createSnepFrame(QList<NDEFRecordList> ndefContainer,snepRequest operation)
{
    // 1) Version.
    QByteArray payload;
    payload.append(0x10);

    // 2) -> Request / Response.
	payload.append((char)operation);

	// 3) Length of ndef message
	quint32 ndefLength = 0;

	NDEFMessage msg(ndefContainer.last());
	ndefLength = msg.toByteArray().count();
	qDebug() << "ndefContainer.length() : "<< ndefLength;

	snepLenUnion length;
	length.lenth4byte = ndefLength;
	payload.append(length.byte[3]);
	payload.append(length.byte[2]);
	payload.append(length.byte[1]);
	payload.append(length.byte[0]);
	
	//qDebug() << "lenSet.lenth4byte : "<< length.lenth4byte;
	//qDebug ("length.byte[0]: %x \nlength.byte[1]: %x",length.byte[0],length.byte[1]);
	//qDebug ("length.byte[2]: %x \nlength.byte[3]: %x",length.byte[2],length.byte[3]);
	//qDebug() << "\n";

    // 4) ndef Payload.
	payload.append(msg.toByteArray());

    return payload;
}

void print_usage(const QString& appName)
{
        err << "Usage: " << appName << " [OUTPUT] OPTIONS" << endl;
        err << "Encode a NDEF Message from OPTIONS." << endl;
        err << "If OUTPUT is not specified, the result is outputed on stdout" << endl << endl;
        err << "Options:" << endl;
        err << "  -t TEXT LOCALE		create new TextRecord" << endl;
        err << "  -u URI			create new UriRecord" << endl;
        err << "  -m MIME-TYPE FILE		create new MimeRecord" << endl;
        err << "  -sp URI			create and open a new SmartPosterRecord" << endl;
        err << "  -s-				close current SmartPoster" << endl;
        err << "  -sa ACTION			create new SpActionRecord" << endl;
        err << "  -ss SIZE			create new SpSizeRecord" << endl;
        err << "  -st TYPE			create new SpTypeRecord" << endl << endl;
        err << "Examples:" << endl;
        err << "  Create a NDEF Message than contains an URL:" << endl;
        err << "    " << appName << " libndef_website.ndef -sp \"http://libndef.googlecode.com\" -t \"libndef\" \"en-US\" -s-" << endl;
        err << "  Create a NDEF Message than contains an electronic card (vCard):" << endl;
        err << "    " << appName << " myvcard.ndef -m \"text/x-vCard\" ./my_vcard.vcf" << endl;
}

typedef enum {
  NDEF_MESSAGE,
  NDEF_SMARTPOSTER,
  NDEF_GENERIC_RECORD,
} ndef_container_type;

int main(int argc, char *argv[])
{
	createSnepMessage(argv[1]);
}

int createSnepMessage(char *filePath)
{
    QList<NDEFRecordList> ndef_containers;
    QList<ndef_container_type> ndef_containers_type;

    //QByteArray libraryPath("../extra/libndef/libndef");
    //qputenv("LD_LIBRARY_PATH", &libraryPath);
    //QByteArray libraryPath;
    //libraryPath = qgetenv ( "LD_LIBRARY_PATH");
    //qDebug() << "snep-encode Library path : " << libraryPath;
    //output.write(libraryPath);
    qDebug() << "\nsnep package creating ... ";

    ndef_containers.append(NDEFRecordList());
    ndef_containers_type.append(NDEF_MESSAGE);

	QStringList arguments(filePath);
	qDebug() << "File path : "<< filePath;
	const QString text = openFile(filePath);
	const QString locale("en-US");
	ndef_containers.last().append(NDEFRecord::createTextRecord(text, locale, NDEFRecord::NDEF_UTF8));

    if (ndef_containers.count() == 0)
    {   // Should not happend...
        err << "Parsing error." << endl;
        return 1;
    }
    if (ndef_containers.count() > 1)
    {
        switch (ndef_containers_type.last())
        {
        case NDEF_SMARTPOSTER:
            err << "A SmartPoster is still opened." << endl;
            break;
        case NDEF_GENERIC_RECORD:
            err << "A GenericControlRecord is still opened." << endl;
            break;
        case  NDEF_MESSAGE:
            err << "You should not see this..." << endl;
            break;
        }
        return 1;
    } // Only root message remains
    if (ndef_containers.at(0).count() == 0)
    {
        err << "There is no NDEF Record to encode." << endl;
        print_usage(arguments.at(0));
        return 1;
    }
    if (!output.isOpen())
    {
        output.open ( QIODevice::WriteOnly );
    }

    QByteArray NFCFrame;
    NFCFrame = createSnepFrame(ndef_containers, snepRequestPut);
    if (output.isOpen ()) {
    	// NDEFMessage contains data
    	output.write(NFCFrame);
     }
    return 0;
}
