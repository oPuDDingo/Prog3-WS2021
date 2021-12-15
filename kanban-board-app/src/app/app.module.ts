import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { ToolbarComponent } from 'src/lib/ui/toolbar/toolbar.component';
import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';

@NgModule({
  declarations: [AppComponent, ToolbarComponent],
  imports: [BrowserModule, AppRoutingModule],
  providers: [],
  bootstrap: [AppComponent],
})
export class AppModule {}
