import _ from 'lodash'
import * as actions from '../actions/canvas'
import SVGFactory from '../helpers/SVGFactory'

export const initialState = {
    lastSvgIndex: -1,
    selectedSvgIndexes: [],
    clipboard: [],
    map: {
        mapData: {
            svg: [],
            actionsIdCounter: 0,
            grid: {
                size: 10,
                enabled: false
            },
            styles: {
                fill: '#626262',
                stroke: '#10cfbd',
                strokeWidth: 4,
                transform: 'rotate(0deg)',
                transformOrigin: '50% 50%'
            }
        }
    }
};

const canvas = (state = initialState, action) => {
    switch (action.type) {
        case actions.ADD_SVG:
            return {
                ...state,
                lastSvgIndex: state.map.mapData.svg.length,
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ...state.map.mapData.svg,
                            action.svg
                        ]
                    }
                }
            };

        case actions.UPDATE_SVG:
            return {
                ...state,
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ..._.slice(state.map.mapData.svg, 0, action.index),
                            action.svg,
                            ..._.slice(state.map.mapData.svg, action.index + 1)
                        ]
                    }
                }
            };

        case actions.REMOVE_SVG:
            var newSvgArray = _.filter(state.map.mapData.svg, (v, i) => {
                return !_.includes(state.selectedSvgIndexes, i);
            });

            return {
                ...state,
                lastSvgIndex: newSvgArray.length - 1,
                selectedSvgIndexes: [],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ...newSvgArray
                        ]
                    }
                }
            };

        case actions.ADD_SVG_TO_SELECTION:
            var selectedMapSvg = _.slice(state.map.mapData.svg, action.index, action.index + 1);
            selectedMapSvg[0].data.selected = true;

            return {
                ...state,
                selectedSvgIndexes: [
                    ...state.selectedSvgIndexes,
                    action.index
                ],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ..._.slice(state.map.mapData.svg, 0, action.index),
                            ...selectedMapSvg,
                            ..._.slice(state.map.mapData.svg, action.index + 1)
                        ]
                    }
                }
            };

        case actions.CLEAR_SVG_SELECTION:
            var newSvgArray = _.map(state.map.mapData.svg, (v, i) => {
                if (_.includes(state.selectedSvgIndexes, i)) {
                    v.data.selected = false;
                }

                return v;
            });

            return {
                ...state,
                selectedSvgIndexes: [],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ...newSvgArray
                        ]
                    }
                }
            };

        case actions.REMOVE_SVG_FROM_SELECTION:
            var selectedMapSvg = state.map.mapData.svg.slice(action.index, action.index + 1);
            selectedMapSvg[0].data.selected = false;

            return {
                ...state,
                selectedSvgIndexes: [
                    ..._.slice(state.selectedSvgIndexes, 0, _.indexOf(state.selectedSvgIndexes, action.index)),
                    ..._.slice(state.selectedSvgIndexes, _.indexOf(state.selectedSvgIndexes, action.index) + 1)
                ],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ..._.slice(state.map.mapData.svg, 0, action.index),
                            ...selectedMapSvg,
                            ..._.slice(state.map.mapData.svg, action.index + 1)
                        ]
                    }
                }
            };

        case actions.SET_SVG_SELECTION:
            var newSvgArray = _.map(state.map.mapData.svg, (v, i) => {
                if (_.includes(state.selectedSvgIndexes, i)) {
                    v.data.selected = false;
                }

                if (action.index == i) {
                    v.data.selected = true;
                }

                return v;
            });

            return {
                ...state,
                selectedSvgIndexes: [action.index],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ...newSvgArray
                        ]
                    }
                }
            };

        case actions.PUSH_SVG_FORWARD:
            if (action.index + 1 >= state.map.mapData.svg.length) {
                return state;
            }

            return {
                ...state,
                selectedSvgIndexes: [action.index + 1],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ..._.slice(state.map.mapData.svg, 0, action.index),
                            ..._.slice(state.map.mapData.svg, action.index + 1, action.index + 2),
                            ..._.slice(state.map.mapData.svg, action.index, action.index + 1),
                            ..._.slice(state.map.mapData.svg, action.index + 2)
                        ]
                    }
                }
            };

        case actions.PUSH_SVG_FRONT:
            if (action.index + 1 >= state.map.mapData.svg.length) {
                return state;
            }

            return {
                ...state,
                selectedSvgIndexes: [state.map.mapData.svg.length - 1],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ..._.slice(state.map.mapData.svg, 0, action.index),
                            ..._.slice(state.map.mapData.svg, action.index + 1),
                            ..._.slice(state.map.mapData.svg, action.index, action.index + 1)
                        ]
                    }
                }
            };

        case actions.PUSH_SVG_BACKWARD:
            if (action.index <= 0) {
                return state;
            }

            return {
                ...state,
                selectedSvgIndexes: [action.index - 1],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ..._.slice(state.map.mapData.svg, 0, action.index - 1),
                            ..._.slice(state.map.mapData.svg, action.index, action.index + 1),
                            ..._.slice(state.map.mapData.svg, action.index - 1, action.index),
                            ..._.slice(state.map.mapData.svg, action.index + 1)
                        ]
                    }
                }
            };

        case actions.PUSH_SVG_BACK:
            if (action.index <= 0) {
                return state;
            }

            return {
                ...state,
                selectedSvgIndexes: [0],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ..._.slice(state.map.mapData.svg, action.index, action.index + 1),
                            ..._.slice(state.map.mapData.svg, 0, action.index),
                            ..._.slice(state.map.mapData.svg, action.index + 1)
                        ]
                    }
                }
            };

        case actions.SET_MAP:
            return {
                ...state,
                map: {
                    ...state.map,
                    ...action.map,
                    mapData: {
                        ...state.map.mapData,
                        ...action.map.mapData
                    }
                }
            };

        case actions.UPDATE_GLOBAL_STYLES:
            return {
                ...state,
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        styles: {
                            ...state.map.mapData.styles,
                            ...action.styles
                        }
                    }
                }
            };

        case actions.UPDATE_GRID:
            return {
                ...state,
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        grid: {
                            ...state.map.mapData.grid,
                            ...action.grid
                        }
                    }
                }
            };

        case actions.CLIPBOARD_COPY:
            return {
                ...state,
                clipboard: [
                    ..._.map(state.selectedSvgIndexes, (index) => {
                        return _.cloneDeep(..._.slice(state.map.mapData.svg, index, index + 1))
                    })
                ]
            };

        case actions.CLIPBOARD_CUT:
            var newClipboardObjects = _.map(state.selectedSvgIndexes, (index) => {
                return _.cloneDeep(..._.slice(state.map.mapData.svg, index, index + 1))
            });

            // Clear existing selected objects
            var newSvgArray = _.filter(state.map.mapData.svg, (v, i) => {
                return !_.includes(state.selectedSvgIndexes, i);
            });

            return {
                ...state,
                clipboard: [
                    ...newClipboardObjects
                ],
                lastSvgIndex: newSvgArray.length - 1,
                selectedSvgIndexes: [],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ...newSvgArray
                        ]
                    }
                }
            };

        case actions.CLIPBOARD_PASTE:
            return {
                ...state,
                selectedSvgIndexes: [
                    ...state.selectedSvgIndexes,
                    ..._.range(state.map.mapData.svg.length, state.map.mapData.svg.length + state.clipboard.length)
                ],
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ...state.map.mapData.svg,
                            ..._.map(state.clipboard, (item) => {
                                return _.cloneDeep(item);
                            })
                        ]
                    }
                }
            };

        case actions.UPDATE_SVG_ACTION:
            // Clone old svg and get state action counter
            var newSvg = _.cloneDeep(state.map.mapData.svg[action.index]);
            var newCounter = state.map.mapData.actionsIdCounter;

            // Check if action already exists and update existing
            if (!_.isEmpty(newSvg.action)) {
                // Update action
                newSvg.action.title = action.title;
                newSvg.action.content = action.content;
            } else {
                // Create new action
                newSvg.action = {
                    id: newCounter,
                    title: action.title,
                    content: action.content
                };

                // Update action ids counter
                newCounter++;
            }

            return {
                ...state,
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        actionsIdCounter: newCounter,
                        svg: [
                            ..._.slice(state.map.mapData.svg, 0, action.index),
                            newSvg,
                            ..._.slice(state.map.mapData.svg, action.index + 1)
                        ]
                    }
                }
            };

        case actions.REMOVE_SVG_ACTION:
            // Clone old svg
            var newSvg = _.cloneDeep(state.map.mapData.svg[action.index]);

            // Remove action
            newSvg.action = {};

            return {
                ...state,
                map: {
                    ...state.map,
                    mapData: {
                        ...state.map.mapData,
                        svg: [
                            ..._.slice(state.map.mapData.svg, 0, action.index),
                            newSvg,
                            ..._.slice(state.map.mapData.svg, action.index + 1)
                        ]
                    }
                }
            };

        default:
            return state;
    }
};

export default canvas
