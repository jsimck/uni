const SVGFactory = {
    Circle: (cx, cy, styles) => {
        return {
            type: 'circle',
            action: {},
            style: {
                ...styles
            },
            data: {
                eventId: null,
                selected: false
            },
            attr: {
                r: 0,
                cx: cx,
                cy: cy
            }
        };
    },
    Ellipse: (cx, cy, styles) => {
        return {
            type: 'ellipse',
            action: {},
            style: {
                ...styles,
                transform: 'rotate(0deg)',
                transformOrigin: '50% 50%'
            },
            data: {
                eventId: null,
                selected: false
            },
            attr: {
                rx: 0,
                ry: 0,
                cx: cx,
                cy: cy
            }
        };
    },
    Rect: (x, y, styles) => {
        return {
            type: 'rect',
            action: {},
            style: {
                ...styles,
                transform: 'rotate(0deg)',
                transformOrigin: '50% 50%'
            },
            data: {
                eventId: null,
                selected: false
            },
            attr: {
                width: 0,
                height: 0,
                x: x,
                y: y,
                rx: 0,
                ry: 0
            }
        };
    },
    Line: (x1, x2, y1, y2, styles) => {
        return {
            type: 'line',
            action: {},
            style: {
                ...styles
            },
            data: {
                eventId: null,
                selected: false
            },
            attr: {
                x1: x1,
                x2: x2,
                y1: y1,
                y2: y2,
                strokeLinecap: 'square'
            }
        };
    },
    Path: (x, y, styles) => {
        return {
            type: 'path',
            action: {},
            style: {
                ...styles
            },
            data: {
                eventId: null,
                lastIndex: 0,
                selected: false
            },
            attr: {
                d: [`M${x} ${y}`]
            }
        };
    },
    Text: (x, y, styles) => {
        return {
            type: 'text',
            action: {},
            style: {
                ...styles,
                fontSize: 0
            },
            data: {
                eventId: null,
                content: 'Hello World',
                selected: false,
                height: 0,
                width: 0
            },
            attr: {
                x: x,
                y: y
            }
        };
    },
    Position: (x, y, eventId = null) => {
        return {
            type: 'position',
            action: {},
            style: {},
            data: {
                selected: false,
                eventId: eventId
            },
            attr: {
                x: x,
                y: y
            }
        };
    }
};

export default SVGFactory