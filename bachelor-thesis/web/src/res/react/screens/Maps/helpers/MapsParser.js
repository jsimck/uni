import _ from 'lodash'

const MapParser = {
    parse: (rawData) => {
        return !_.isEmpty(rawData) ? rawData : [];
    },
    parseSingle: (rawData) => {
        if (!_.isEmpty(rawData)) {
            rawData.mapData = JSON.parse(rawData.mapData);

            // Parse while mapData is not object
            while (typeof rawData.mapData !== 'object') {
                rawData.mapData = JSON.parse(rawData.mapData);
            }

            // Set 'selected' attribute from all svgs to false,
            // so there are no selections upon loading
            rawData.mapData.svg = _.map(rawData.mapData.svg, (svg) => {
                svg.data.selected = false;
                return svg;
            });

            return rawData;
        }

        return {};
    }
};

export default MapParser;