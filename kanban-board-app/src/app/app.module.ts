import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { BrowserModule } from '@angular/platform-browser';
import { BoardComponent } from 'src/lib/ui/board/board.component';
import { ColumnComponent } from 'src/lib/ui/column/column.component';
import { ItemComponent } from 'src/lib/ui/item/item.component';
import { ToolbarComponent } from '../lib/ui/toolbar/toolbar.component';
import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';

@NgModule({
  declarations: [
    AppComponent,
    ToolbarComponent,
    BoardComponent,
    ColumnComponent,
    ItemComponent,
  ],
  imports: [BrowserModule, AppRoutingModule, FormsModule],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule {}
