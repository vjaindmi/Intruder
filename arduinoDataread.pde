import processing.serial.*;
Serial myPort;
Table dataTable;

int numReadings = 5;
int readingCounter = 0;

String fileName;
void setup()
{
  String portName = Serial.list()[0]; 

  myPort = new Serial(this, portName, 9600);
  dataTable = new Table(); 
  dataTable.addColumn("id");

  dataTable.addColumn("year");
  dataTable.addColumn("month");
  dataTable.addColumn("day");
  dataTable.addColumn("hour");
  dataTable.addColumn("minute");
  dataTable.addColumn("second");

  dataTable.addColumn("PIR Sensor");
  dataTable.addColumn("Smoke Sensor");
}

void serialEvent(Serial myPort) {
  String val = myPort.readStringUntil('\n');
  if (val!= null) {
    val = trim(val);
    println(val);
    float sensorVals[] = float(split(val, ','));
    println("PIR = "+sensorVals[0]);
    println("Smoke = "+sensorVals[1]);
    TableRow newRow = dataTable.addRow();
    newRow.setInt("id", dataTable.lastRowIndex());

    newRow.setInt("year", year());
    newRow.setInt("month", month());
    newRow.setInt("day", day());
    newRow.setInt("hour", hour());
    newRow.setInt("minute", minute());
    newRow.setInt("second", second());

    newRow.setFloat("Smoke Sensor", sensorVals[0]);
    newRow.setFloat("PIR Sensor", sensorVals[1]);

    readingCounter++;

   if (readingCounter % numReadings ==0)
    {
      fileName = str(year()) + str(month()) + str(day()) + str(dataTable.lastRowIndex());
      saveTable(dataTable, fileName);
    }
  }
}