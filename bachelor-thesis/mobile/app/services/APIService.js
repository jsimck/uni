import _ from 'lodash'

// Debug API endpoint URL
//const BASE_URL = 'http://192.168.0.101:8000/api/v1/';
// Release API Endpoint url
const BASE_URL = 'http://mrl.cs.vsb.cz:80/pressapp/api/v1/';

const APIService = {
    listEvents: (callback) => {
        fetch(BASE_URL + 'events/', {
                method: 'GET'
            })
        .then(response => {
            if ([403, 404].indexOf(response.status) !== -1) {
                throw new Error('Chyba aktualize událostí');
            }

            return response.json();
        })
        .then(json => {
            callback(true, json);
        })
        .catch(error => {
            callback(false, error);
        });
    },
    getEvent: (id, callback) => {
        fetch(BASE_URL + `events/${id}`, {
                method: 'GET'
            })
        .then(response => {
            if ([403, 404].indexOf(response.status) !== -1) {
                throw new Error('Chyba aktualize události');
            }

            return response.json();
        })
        .then(json => {
            callback(true, json);
        })
        .catch(error => {
            callback(false, error);
        });
    },
    listMaps: (callback) => {
        fetch(BASE_URL + 'maps/', {
                method: 'GET'
            })
        .then(response => {
            if ([403, 404].indexOf(response.status) !== -1) {
                throw new Error('Chyba aktualize map');
            }

            return response.json();
        })
        .then(json => {
            callback(true, json);
        })
        .catch(error => {
            callback(false, error);
        });
    },
    getMap: (id, callback) => {
        fetch(BASE_URL + `maps/${id}`, {
                method: 'GET'
            })
        .then(response => {
            if ([403, 404].indexOf(response.status) !== -1) {
                throw new Error('Chyba aktualize mapy');
            }

            return response.json();
        })
        .then(json => {
            // Parse mapData
            json.mapData = JSON.parse(json.mapData);
            callback(true, json);
        })
        .catch(error => {
            callback(false, error);
        });
    }
};

export default APIService