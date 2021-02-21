import React, { Component, StyleSheet, View, Modal, Text, TouchableOpacity, Linking, Platform } from 'react-native'
import Icon from '../../node_modules/react-native-vector-icons/EvilIcons'
import MaterialIcon from '../../node_modules/react-native-vector-icons/MaterialIcons'
import _ from 'lodash'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { qrReaderSelector } from '../selectors/qrReaderSelector'
import { showQrReader } from '../actions/qrReaderActions'
import { fetchMap } from '../actions/eventsActions'
import { setAction } from '../actions/actionActions'
import { pushAlert } from '../actions/notificationAlertActions'
import { updateGame, addGame } from '../actions/gamesActions'
import { openGameModal, GAME_MODAL_IMG_WON, GAME_MODAL_IMG_ERROR, GAME_MODAL_IMG_INFO, GAME_MODAL_IMG_SCAN } from '../actions/gameModalActions'
import { closeQrReader, setQrReaderRead } from '../actions/qrReaderActions'
import CameraView from '../components/CameraView'
import EventsScreen from './EventsScreen'
import EventScreen from './EventScreen'
import ActionScreen from './ActionScreen'
import GameModal from './GameModal'

class QrReaderModal extends Component
{
    getAction(eventId, actionId, mapId) {
        var event = this.getEvent(eventId);

        // Check if map exist
        if (_.isEmpty(event.map)) {
            this.props.fetchMap(mapId, eventId);
            this.props.pushAlert('Mapa nenalezena', 'Mapa zatím nebyla stažena ze serveru, prosím opakujte akci znovu');
            return false;
        }

        // Get action from map data
        var action = _.find(event.map.mapData.svg, (event) => {
            return event.action.id == actionId;
        }).action;

        // Check if action exist
        if (_.isEmpty(action)) {
            this.props.pushAlert('Chybná Akce', 'Načtená akce nebyla nalezena');
            return false;
        }

        // Return action
        return action;
    }

    getEvent(eventId) {
        var event = _.find(this.props.events, {id: eventId});

        // Check if event exists
        if (_.isEmpty(event)) {
            this.props.pushAlert('Chybná Události', 'Načtená událost nebyla nalezena');
            return false;
        }

        return event;
    }

    handleOnBarCodeRead(event) {
        // Return if we already read QR code
        if (this.props.qrRead) {
            return;
        }

        // Parse QR data
        try {
            var qrData = JSON.parse(event.data);
        } catch (error) {
            // Open url
            Linking.openURL(event.data);
            return;
        }

        // Set qr code read to true
        this.props.setQrReaderRead(true);

        // Do action based on type
        switch(qrData.type) {
            case 'position':
                // Get event by ID
                var event = this.getEvent(qrData.data.eventId);

                // Check if event exists
                if (event) {
                    // Push new view
                    this.props.navigator.push({
                        title: event.title,
                        name: 'event',
                        component: EventScreen,
                        data: {
                            eventId: event.id,
                            position: {
                                x: qrData.data.x,
                                y: qrData.data.y
                            }
                        }
                    });

                    // Close modal window
                    this.props.closeQrReader();
                }
                break;

            case 'event':
                // Get event by ID
                var event = this.getEvent(qrData.data.id);

                // Check if event exists
                if (event) {
                    // Push new view
                    this.props.navigator.push({
                        title: event.title,
                        name: 'event',
                        component: EventScreen,
                        data: {
                            eventId: event.id
                        }
                    });

                    // Close modal window
                    this.props.closeQrReader();
                }
                break;

            case 'action':
                // Set selected action
                var event = this.getEvent(qrData.data.eventId);
                var action = this.getAction(qrData.data.eventId, qrData.data.actionId, qrData.data.mapId);

                // Check if action exists
                if (action) {
                    // Set action
                    this.props.setAction(action);

                    // Close modal window
                    this.props.closeQrReader();

                    // Push new Event view
                    this.props.navigator.push({
                        title: event.title,
                        name: 'event',
                        component: EventScreen,
                        data: {
                            eventId: event.id
                        }
                    });

                    // Push new Action view
                    this.props.navigator.push({
                        title: action.title,
                        name: 'action',
                        component: ActionScreen
                    });
                }
                break;

            case 'game':
                // Get event and game if exists
                var event = this.getEvent(qrData.data.eventId);

                // Check if game is defined on the event
                if (_.isNull(event.game)) {
                    this.props.pushAlert('Hra nenalezena', 'Prosím aktualizujte seznam událostí a zkuste tento kód načíst znovu');
                } else {
                    var game = _.find(this.props.games, {id: qrData.data.eventId});
                    // Check if game exists
                    if (_.isUndefined(game)) {
                        // Doesn't exists, so we will call 'add' action and create it
                        this.props.addGame(qrData.data.eventId, qrData.data.qrId);
                        this.props.openGameModal('Soutěž - ' + event.game.title, 'Gratulujeme! Právě jste nasbíral první z ' + event.game.qrCount + ' QR kódů nutných k dokončení soutěže. Pro více informací klikněte na tlačítko níže.', GAME_MODAL_IMG_INFO, event);
                    } else {
                        // Game exists, check if it's not already completed
                        if (game.qrIds.length >= event.game.qrCount) {
                            this.props.openGameModal('Už jste vyhrál!', event.game.winningMessage, GAME_MODAL_IMG_WON, event);
                        } else if (_.includes(game.qrIds, qrData.data.qrId)) {
                            // THis qr code was already saved into the store, so we won't update anything
                            this.props.openGameModal('Špatný QR kód!', 'Litujeme, ale tento QR kód jste již načetl. Nezoufejte a hledejte dál!', GAME_MODAL_IMG_ERROR, event);
                        } else {
                            // Unique qr code, update game reducer
                            game = _.cloneDeep(game);

                            // Push new id
                            game.qrIds.push(qrData.data.qrId);

                            // Update reducer
                            this.props.updateGame(game);

                            // Check if user won
                            if (game.qrIds.length >= event.game.qrCount) {
                                // Show progress message
                                this.props.openGameModal('Vyhrál jste!', event.game.winningMessage, GAME_MODAL_IMG_WON, event);
                            } else {
                                // Show progress message
                                this.props.openGameModal('Našel jste další!', 'Už vám zbývá nasbírat pouze ' + (event.game.qrCount - game.qrIds.length) + '/' + event.game.qrCount + ' QR kódů pro dokončení soutěže!', GAME_MODAL_IMG_SCAN, event);
                            }
                        }
                    }
                }
                break;
        }
    }

    render() {
        var platform = Platform.OS;

        return (
            <Modal
                style={styles.container}
                visible={this.props.visible}
                onRequestClose={() => this.props.closeQrReader()}
                animated={true}
            >
                <View style={styles.topBar}>
                    {platform == 'ios' &&
                        <View style={styles.topBarItem}>

                        </View>
                    }
                    <View style={styles.topBarItem}>
                        <Text style={styles['navBarText' + platform]}>
                            Čtečka QR kódů
                        </Text>
                    </View>
                    <View style={[styles.topBarItem, styles.topBarItemRight]}>
                        <TouchableOpacity
                            onPress={() => this.props.closeQrReader()}
                            style={styles['navBarButtonClose' + platform]}>
                            {platform == 'ios' && <Icon style={styles.navBarButtonIcon} name="close" size={25} />}
                            {platform == 'android' && <MaterialIcon style={styles.navBarButtonIcon} name="close" size={25} />}
                        </TouchableOpacity>
                    </View>
                </View>
                <CameraView onBarCodeRead={(event) => this.handleOnBarCodeRead(event)} />
                <GameModal navigator={this.props.navigator} />
            </Modal>
        );
    }
}

const styles = StyleSheet.create({
    container: {
        flex: 1
    },
    topBar: {
        paddingTop: 24,
        height: (Platform.OS === 'ios') ? 64 : 56,
        flexDirection: 'row',
        backgroundColor: '#21bfbd'
    },
    topBarItem: {
        flex: 1
    },
    topBarItemRight: {
        alignItems: 'flex-end'
    },
    navBarButtonCloseios: {
        marginTop: -11,
        marginRight: -10,
        padding: 20
    },
    navBarButtonCloseandroid: {
        marginTop: -26,
        marginRight: -3,
        padding: 20
    },
    navBarTextios: {
        marginTop: 9,
        fontSize: 16,
        fontFamily: 'Source Sans Pro',
        fontWeight: 'normal',
        textAlign: 'center',
        color: '#222'
    },
    navBarTextandroid: {
        marginTop: -6,
        marginLeft: -4,
        textAlign: 'center',
        color: '#222',
        fontSize: 20,
        fontWeight: '500'
    },
    navBarButtonIcon: {
        color: '#222'
    }
});

QrReaderModal.propTypes = {
    navigator: React.PropTypes.object,
    events: React.PropTypes.array,
    games: React.PropTypes.array,
    qrRead: React.PropTypes.bool,
    qrReaderSelector: React.PropTypes.bool,

    // Functions
    setAction: React.PropTypes.func,
    pushAlert: React.PropTypes.func,
    setQrReaderRead: React.PropTypes.func,
    fetchMap: React.PropTypes.func,
    updateGame: React.PropTypes.func,
    addGame: React.PropTypes.func,
    showQrReader: React.PropTypes.func,
    openGameModal: React.PropTypes.func,
    closeQrReader: React.PropTypes.func
};

export default connect(
    qrReaderSelector,
    (dispatch) => {
        return bindActionCreators({ closeQrReader, setAction, pushAlert, setQrReaderRead, fetchMap, showQrReader, updateGame, addGame, openGameModal }, dispatch);
    }
)(QrReaderModal)