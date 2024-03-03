
function onButtonClick() {
    console.log('Requesting Bluetooth Device...');
    navigator.bluetooth.requestDevice({filters: [{services: ['c5816eb9-773f-4478-aa3b-c466da9ec23d']}]})
    .then(device => {
      console.log('Connecting to GATT Server...');
      return device.gatt.connect();
    })
    .then(server => {
      console.log('Getting Heart Rate Service...');
      return server.getPrimaryService('c5816eb9-773f-4478-aa3b-c466da9ec23d');
    })
    .then(service => {
      console.log('Getting Heart Rate Control Point Characteristic...');
      return service.getCharacteristic('a290ac53-b59f-4254-b910-c5044e1942c8');
    })
    .then(characteristic => {
      console.log('Writing Heart Rate Control Point Characteristic...');
  
      // Writing 1 is the signal to reset energy expended.
      let resetEnergyExpended = Uint8Array.of(1);
      return characteristic.writeValue(resetEnergyExpended);
    })
    .then(() => {log('> Energy expended has been reset.');})
    .catch(error => {
      console.log('Argh! ' + error);
    });
  }