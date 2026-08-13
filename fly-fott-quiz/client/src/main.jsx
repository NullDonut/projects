import React from 'react';
import { createRoot } from 'react-dom/client';
import { BrowserRouter, Routes, Route, Navigate } from 'react-router-dom';
import Judge from './pages/Judge.jsx';
import Player from './pages/Player.jsx';
import './styles.css';

createRoot(document.getElementById('root')).render(
  <React.StrictMode>
    <BrowserRouter>
      <Routes>
        <Route path="/judge" element={<Judge />} />
        <Route path="/play/:roomCode" element={<Player />} />
        <Route path="/play" element={<Player />} />
        <Route path="*" element={<Navigate to="/judge" replace />} />
      </Routes>
    </BrowserRouter>
  </React.StrictMode>
);
