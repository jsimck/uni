import _ from 'lodash'

const PathHelpers = {
    parseStringPoint: (point) => {
        // Split point string to coordinates
        point = point.replace(/[^\d\.\-\s]/g, '');
        point = point.split(' ');

        // Parse coordinates to integer
        point[0] = parseInt(point[0]);
        point[1] = parseInt(point[1]);

        // Return parsed point
        return point;
    },
    getPointsArray: (points) => {
        return _.map(points, (entry) => {
            return PathHelpers.parseStringPoint(entry);
        });
    },
    getPathPoint: (modifierKey, prevPoint, nextPoint) => {
        if (modifierKey) {
            // Calculate line degree
            var deltaX = nextPoint[0] - prevPoint[0];
            var deltaY = nextPoint[1] - prevPoint[1];
            // Get angle in absolute position
            var angle = Math.atan2(deltaX, deltaY) * 180 / Math.PI;
            var absAngle = Math.abs(angle);

            // Apply vertical/horizontal line flag based on angle
            if ((absAngle >= 0 && absAngle < 30) || (absAngle >= 150 && absAngle < 180)) {
                return `L${prevPoint[0]} ${nextPoint[1]}`;
            } else if ((absAngle >= 60 && absAngle < 120)) {
                return `L${nextPoint[0]} ${prevPoint[1]}`;
            } else if ((angle >= 30 && angle < 60)) {
                return `L${prevPoint[0] + deltaX} ${prevPoint[1] + deltaX}`;
            } else if ((angle >= 120 && angle < 150)) {
                return `L${prevPoint[0] - deltaY} ${prevPoint[1] + deltaY}`;
            } else if ((angle <= -120 && angle > -150)) {
                return `L${prevPoint[0] + deltaY} ${prevPoint[1] + deltaY}`;
            } else {
                return `L${prevPoint[0] + deltaX} ${prevPoint[1] - deltaX}`;
            }
        }

        return `L${nextPoint[0]} ${nextPoint[1]}`;
    }
};

export default PathHelpers