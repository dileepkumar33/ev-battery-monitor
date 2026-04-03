importScripts('https://www.gstatic.com/firebasejs/9.0.0/firebase-app-compat.js');
importScripts('https://www.gstatic.com/firebasejs/9.0.0/firebase-messaging-compat.js');

const firebaseConfig = {
  apiKey: "AIzaSyAhJreEklRMJvrDA0Ikks3t_2033HJskrY",
  authDomain: "ai-agent-rooms.firebaseapp.com",
  databaseURL: "https://ai-agent-rooms-default-rtdb.firebaseio.com",
  projectId: "ai-agent-rooms",
  storageBucket: "ai-agent-rooms.firebasestorage.app",
  messagingSenderId: "140279941161",
  appId: "1:140279941161:web:66786c89c0942f7bc4f438",
  measurementId: "G-CC137LWWSJ"
};

firebase.initializeApp(firebaseConfig);

const messaging = firebase.messaging();

messaging.onBackgroundMessage((payload) => {
  console.log('[firebase-messaging-sw.js] Received background message ', payload);
  const notificationTitle = payload.notification.title;
  const notificationOptions = {
    body: payload.notification.body,
    icon: '/vite.svg'
  };

  self.registration.showNotification(notificationTitle, notificationOptions);
});
