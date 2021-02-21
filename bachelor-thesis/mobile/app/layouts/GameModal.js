import React, { Component, StyleSheet, View, Modal, Text, Image, TouchableOpacity, Platform, ScrollView } from 'react-native'
import Icon from '../../node_modules/react-native-vector-icons/EvilIcons'
import MaterialIcon from '../../node_modules/react-native-vector-icons/MaterialIcons'
import _ from 'lodash'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { gamesModalSelector } from '../selectors/gamesModalSelector'
import { closeGameModal, GAME_MODAL_IMG_INFO, GAME_MODAL_IMG_SCAN, GAME_MODAL_IMG_WON } from '../actions/gameModalActions'
import { closeQrReader } from '../actions/qrReaderActions'
import EventScreen from '../layouts/EventScreen'

class GameModal extends Component
{
    constructor(props) {
        super(props);

        this.state = {
            progress: 0.2
        };
    }

    getHeadlineImage(type) {
        switch (type) {
            case GAME_MODAL_IMG_INFO:
                return <Image style={styles.headlineImage} source={require('../img/game_info.png')} />;

            case GAME_MODAL_IMG_SCAN:
                return <Image style={styles.headlineImage} source={require('../img/game_scan.png')} />;

            case GAME_MODAL_IMG_WON:
                return <Image style={styles.headlineImage} source={require('../img/game_won.png')} />;

            default:
                return <Image style={styles.headlineImage} source={require('../img/game_error.png')} />;
        }
    }

    getBackgroundStyles(type) {
        switch (type) {
            case GAME_MODAL_IMG_INFO:
                return [styles.content, styles.contentInfo];

            case GAME_MODAL_IMG_SCAN:
                return [styles.content, styles.contentScan];

            case GAME_MODAL_IMG_WON:
                return [styles.content, styles.contentWon];

            default:
                return styles.content;
        }
    }

    handleOnInfoPress() {
        // Close game modal & QR modal
        this.props.closeGameModal();
        this.props.closeQrReader();

        // Push New View
        setTimeout(() => {
            this.props.navigator.push({
                title: this.props.event.title,
                name: 'event',
                component: EventScreen,
                data: {
                    eventId: this.props.event.id,
                    selectedTab: 'soutez'
                }
            });
        }, 1000);
    }

    render() {
        var platform = Platform.OS;

        return (
            <Modal
                style={styles.modal}
                visible={this.props.visible}
                onRequestClose={() => this.props.closeGameModal()}
                animated={true}
                transparent={true}
            >
                <View style={styles.container}>
                    <ScrollView>
                        <View style={this.getBackgroundStyles(this.props.type)}>
                            <TouchableOpacity style={styles.closeButtonContainer} onPress={() => this.props.closeGameModal()}>
                                {platform == 'ios' && <Icon style={styles.closeButtonios} name="close" size={25} />}
                                {platform == 'android' && <MaterialIcon style={styles.closeButtonandroid} name="close" size={25} />}
                            </TouchableOpacity>
                            {this.getHeadlineImage(this.props.type)}
                            <Text style={styles.headline}>{this.props.title}</Text>
                            <Text style={styles.text}>{this.props.text}</Text>
                            <TouchableOpacity onPress={() => this.handleOnInfoPress()} style={styles.buttonContainer}>
                                <Text style={styles.buttonText}>Další informace</Text>
                            </TouchableOpacity>
                        </View>
                    </ScrollView>
                </View>
            </Modal>
        );
    }
}

const styles = StyleSheet.create({
    modal: {
        flex: 1
    },
    container: {
        flex: 1,
        flexDirection: 'row',
        marginHorizontal: 20,
    },
    content: {
        flex: 1,
        flexDirection: 'column',
        overflow: 'hidden',
        backgroundColor: '#f9ab6a',
        marginVertical: 20,
        borderRadius: 3,
        paddingTop: 15,
        paddingBottom: 25,
        paddingHorizontal: 25
    },
    contentInfo: {
        backgroundColor: '#f0eed3'
    },
    contentScan: {
        backgroundColor: '#84F9E1'
    },
    contentWon: {
        backgroundColor: '#c8e8f1'
    },
    closeButtonandroid: {
        color: '#222',
        textAlign: 'right',
        marginRight: -5,
        marginTop: 5
    },
    closeButtonios: {
        color: '#222',
        textAlign: 'right',
        marginRight: -13
    },
    closeButtonContainer: {
        flex: 1,
        flexDirection: 'row',
        justifyContent: 'flex-end',
        alignItems: 'flex-end'
    },
    headlineImage: {
        alignSelf: 'center',
        marginBottom: 25
    },
    headline: {
        fontWeight: '500',
        textAlign: 'center',
        color: '#222',
        fontFamily: 'Source Sans Pro',
        marginBottom: 5,
        fontSize: 22
    },
    text: {
        fontFamily: 'Source Sans Pro',
        fontWeight: '400',
        textAlign: 'center',
        color: '#444',
        fontSize: 14,
        lineHeight: 21
    },
    buttonContainer: {
        flex: 1,
        marginTop: 15,
        flexDirection: 'row',
        alignSelf: 'stretch',
        alignItems: 'center',
        justifyContent: 'center',
        borderColor: '#222',
        height: 46,
        borderRadius: 23,
        borderWidth: 1
    },
    buttonText: {
        fontFamily: 'Source Sans Pro',
        fontWeight: '400',
        textAlign: 'center',
        color: '#222',
        fontSize: 16
    }
});

GameModal.propTypes = {
    event: React.PropTypes.object,
    visible: React.PropTypes.bool,
    title: React.PropTypes.string,
    text: React.PropTypes.string,
    type: React.PropTypes.string,

    // Functions
    closeGameModal: React.PropTypes.func,
    closeQrReader: React.PropTypes.func
};

export default connect(
    gamesModalSelector,
    (dispatch) => {
        return bindActionCreators({ closeGameModal, closeQrReader }, dispatch);
    }    
)(GameModal)