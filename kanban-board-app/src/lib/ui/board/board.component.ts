import { Component } from '@angular/core';
import { ColumnModel } from '../../../lib/data-access/models/columnModel';

@Component({
  selector: 'board',
  templateUrl: './board.component.html',
  styleUrls: ['./board.component.scss'],
})
export class BoardComponent {
  // @Input()
  // title = 'Let me guide you!';

  columns: ColumnModel[] = [];

  addFunction() {
    const unixTime = 1210981217;
    const date = new Date(unixTime * 1000);
    var tmp = date.toLocaleDateString('en-US');
    this.columns.push({
      id: (Math.random() * 10000) | 0,
      name: '',
      position: 0,
      items: [
        {
          id: 0,
          title: '',
          timestamp: tmp,
        },
      ],
    });
  }

  addItem(column: ColumnModel) {
    const unixTime = 1210981217;
    const date = new Date(unixTime * 1000);
    var tmp = date.toLocaleDateString('en-US');
    column.items.push({
      id: 0,
      title: '',
      timestamp: tmp,
    });
  }

  changeTitle(column: ColumnModel, event) {
    column.name = event;
    // console.log(column.name);
  }

  deleteFunction(column: ColumnModel) {
    this.columns.splice(this.columns.indexOf(column), 1);
  }
}

// [
//   {
//     id: 0,
//     name: 'to do',
//     position: 0,
//     items: [
//       {
//         id: 0,
//         title: 'first task',
//         position: 0,
//         timestamp: '2021-12-09T10:04:42.058Z',
//       },
//       {
//         id: 1,
//         title: 'second task',
//         position: 1,
//         timestamp: '2021-12-09T10:04:42.058Z',
//       },
//       {
//         id: 2,
//         title: 'third task',
//         position: 2,
//         timestamp: '2021-12-09T10:04:42.058Z',
//       },
//     ],
//   },

//   {
//     id: 1,
//     name: 'doing',
//     position: 1,
//     items: [
//       {
//         id: 0,
//         title: 'my special first task',
//         position: 0,
//         timestamp: '2021-12-09T10:04:42.058Z',
//       },
//     ],
//   },

//   // {
//   //   id: 2,
//   //   name: 'done',
//   //   position: 2,
//   //   items: [
//   //     {
//   //       id: 0,
//   //       title: 'first task',
//   //       position: 0,
//   //       timestamp: '2021-12-09T10:04:42.058Z',
//   //     },
//   //     {
//   //       id: 1,
//   //       title: 'second task',
//   //       position: 1,
//   //       timestamp: '2021-12-09T10:04:42.058Z',
//   //     },
//   //   ],
//   // },
