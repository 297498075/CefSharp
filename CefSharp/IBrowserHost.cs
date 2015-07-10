﻿// Copyright © 2010-2015 The CefSharp Authors. All rights reserved.
//
// Use of this source code is governed by a BSD-style license that can be found in the LICENSE file.

using System;
using System.Threading.Tasks;

namespace CefSharp
{
    public interface IBrowserHost : IDisposable
    {
        void StartDownload(string url);
        void Print();
        void SetZoomLevel(double zoomLevel);
        Task<double> GetZoomLevelAsync();
        IntPtr GetWindowHandle();
        void CloseBrowser(bool forceClose);
        
        void ShowDevTools();
        void CloseDevTools();

        void AddWordToDictionary(string word);
        void ReplaceMisspelling(string word);

        void Find(int identifier, string searchText, bool forward, bool matchCase, bool findNext);
        void StopFinding(bool clearSelection);

        /// <summary>
        /// Send a mouse wheel event to the browser.
        /// </summary>
        /// <param name="x">X-Axis coordinate relative to the upper-left corner of the view.</param>
        /// <param name="y">Y-Axis coordinate relative to the upper-left corner of the view.</param>
        /// <param name="deltaX">Movement delta for X direction.</param>
        /// <param name="deltaY">movement delta for Y direction.</param>
        void SendMouseWheelEvent(int x, int y, int deltaX, int deltaY);

        /// <summary>
        /// Invalidate the view. The browser will call CefRenderHandler::OnPaint asynchronously.
        /// This method is only used when window rendering is disabled (OSR). 
        /// </summary>
        /// <param name="type">indicates which surface to re-paint either View or Popup.</param>
        void Invalidate(PaintElementType type);

        /// <summary>
        /// Send a mouse click event to the browser.
        /// </summary>
        /// <param name="x">x coordinate - relative to upper-left corner of view</param>
        /// <param name="y">y coordinate - relative to upper-left corner of view</param>
        /// <param name="mouseButtonType">Mouse ButtonType</param>
        /// <param name="mouseUp">mouse up</param>
        /// <param name="clickCount">click count</param>
        /// <param name="modifiers">click modifiers e.g. Ctrl</param>
        void SendMouseClickEvent(int x, int y, MouseButtonType mouseButtonType, bool mouseUp, int clickCount, CefEventFlags modifiers);

        /// <summary>
        /// Send a mouse move event to the browser
        /// </summary>
        /// <param name="x">x coordinate - relative to upper-left corner of view</param>
        /// <param name="y">y coordinate - relative to upper-left corner of view</param>
        /// <param name="mouseLeave">mouse leave</param>
        /// <param name="modifiers">click modifiers .e.g Ctrl</param>
        void SendMouseMoveEvent(int x, int y, bool mouseLeave, CefEventFlags modifiers);

        bool IsWindowRenderingDisabled();
    }
}
